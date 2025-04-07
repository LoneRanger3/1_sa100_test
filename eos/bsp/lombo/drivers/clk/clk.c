/*
 * clk.c - Standard functionality for the common clock API.
 *
 * Copyright (C) 2016-2018, LomboTech Co.Ltd.
 * Author: lomboswer <lomboswer@lombotech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <clk_debug.h>
#include <rtthread.h>
#include "csp.h"
#include "clk.h"
#include "clk_csp.h"
#include "clk_private.h"

HLIST_HEAD(clk_root_list);
HLIST_HEAD(clk_orphan_list);

clk_t g_clk_src[CLK_NUM];
struct rt_mutex clk_lock;
#define CLK_HANDLE_MAGIC 0xA000
#define CLK_HANDLE_MAGIC_MASK 0xF000

clk_t *handle_to_clk(clk_handle_t handle)
{
	if ((handle & CLK_HANDLE_MAGIC_MASK) != CLK_HANDLE_MAGIC)
		return NULL;

	handle = handle & ~CLK_HANDLE_MAGIC_MASK;
	if (handle < CLK_ID_START || handle >= CLK_NUM)
		return NULL;
	return &g_clk_src[handle];
}

clk_handle_t clk_to_handle(clk_t *clk)
{
	clk_handle_t handle = -EINVAL;
	if (clk == NULL || clk->hw == NULL)
		return handle;
	handle = (clk_handle_t)(clk->hw->id) | CLK_HANDLE_MAGIC;
	return handle;
}

static void clk_ops_lock(void)
{
	if (rt_thread_self() != NULL)
		rt_mutex_take(&clk_lock, RT_WAITING_FOREVER);
}

static void clk_ops_unlock(void)
{
	if (rt_thread_self() != NULL)
		rt_mutex_release(&clk_lock);
}

static int check_clk(clk_t *clk)
{
	if ((clk == NULL) || (clk->hw == NULL))
		return RT_EINVAL;
	return RT_EOK;
}
#define LOOKUP_PARENT_BY_INDEX(clk, index) \
	(get_clk_from_id(clk->hw->clk_src[index]))

clk_t *__clk_get_parent(clk_t *clk)
{
	return check_clk(clk) ? NULL : clk->parent;
}

u32 __clk_get_rate(clk_t *clk)
{
	u32 ret;

	if (check_clk(clk) != 0) {
		ret = 0;
		goto out;
	}

	ret = clk->rate;

	if (get_clk_flag(clk) & CLK_IS_ROOT)
		goto out;

	if (!clk->parent)
		ret = 0;

out:
	return ret;
}

static void __clk_disable(clk_t *clk)
{
	int ret = 0;

	ret = check_clk(clk);
	if (ret != 0)
		return;

	if (clk->enable_count == 0) {
		/* LOG_W("%s %d: disable a clock whose enable_count is zero\n",
			__FILE__, __LINE__); */
		return;
	}

	if (--clk->enable_count > 0)
		return;

	if (clk->ops->disable)
		clk->ops->disable(clk->hw);

	__clk_disable(clk->parent);
}

/**
 * clk_disable - gate a clock
 * @handle: the clk being gated
 *
 * clk_disable must not sleep, which differentiates it from clk_unprepare.  In
 * a simple case, clk_disable can be used instead of clk_unprepare to gate a
 * clk if the operation is fast and will never sleep.  One example is a
 * SoC-internal clk which is controlled via simple register writes.  In the
 * complex case a clk gate operation may require a fast and a slow part.  It is
 * this reason that clk_unprepare and clk_disable are not mutually exclusive.
 * In fact clk_disable must be called before clk_unprepare.
 */
void clk_disable(clk_handle_t handle)
{
	clk_t *clk = handle_to_clk(handle);
	clk_ops_lock();
	__clk_disable(clk);
	clk_ops_unlock();
}

static int __clk_enable(clk_t *clk)
{
	int ret = 0;

	ret = check_clk(clk);
	if (ret != 0)
		return 0;
	if (clk->hw->id == CLK_ID_NULL_CLK) {
		LOG_E("Err: enabling null clk. Please set parent first!");
		return -1;
	}

	if (clk->enable_count == 0) {
		ret = __clk_enable(clk->parent);
		if (ret)
			return ret;

		if (clk->ops->enable) {
			ret = clk->ops->enable(clk->hw);
			if (ret) {
				__clk_disable(clk->parent);
				return ret;
			}
		}
	}

	clk->enable_count++;
	return 0;
}

/**
 * clk_enable - ungate a clock
 * @handle: the clk being ungated
 *
 * clk_enable must not sleep, which differentiates it from clk_prepare.  In a
 * simple case, clk_enable can be used instead of clk_prepare to ungate a clk
 * if the operation will never sleep.  One example is a SoC-internal clk which
 * is controlled via simple register writes.  In the complex case a clk ungate
 * operation may require a fast and a slow part.  It is this reason that
 * clk_enable and clk_prepare are not mutually exclusive.  In fact clk_prepare
 * must be called before clk_enable.  Returns 0 on success, -EERROR
 * otherwise.
 */
int clk_enable(clk_handle_t handle)
{
	int ret;
	clk_t *clk = handle_to_clk(handle);

	clk_ops_lock();
	ret = __clk_enable(clk);
	clk_ops_unlock();

	return ret;
}

/**
 * __clk_round_rate - round the given rate for a clk
 * @clk: round the rate of this clock
 *
 * Caller must hold prepare_lock.  Useful for clk_ops such as .set_rate
 */
u32 __clk_round_rate(clk_t *clk, u32 rate)
{
	u32 parent_rate = 0;

	if (!clk)
		return 0;

	if (!clk->ops->round_rate) {
		if (get_clk_flag(clk) & CLK_SET_RATE_PARENT)
			return __clk_round_rate(clk->parent, rate);
		else
			return clk->rate;
	}

	if (clk->parent)
		parent_rate = clk->parent->rate;

	return clk->ops->round_rate(clk->hw, rate, &parent_rate);
}

/**
 * __clk_notify - call clk notifier chain
 * @clk: clk_t * that is changing rate
 * @msg: clk notifier type (see include/linux/clk.h)
 * @old_rate: old clk rate
 * @new_rate: new clk rate
 *
 * Triggers a notifier call chain on the clk rate-change notification
 * for 'clk'.  Passes a pointer to the clk_t and the previous
 * and current rates to the notifier callback.  Intended to be called by
 * internal clock code only.  Returns NOTIFY_DONE from the last driver
 * called if all went well, or NOTIFY_STOP or NOTIFY_BAD immediately if
 * a driver returns that.
 */
static int __clk_notify(clk_t *clk, u32 msg,
		u32 old_rate, u32 new_rate)
{
	struct clk_notifier_data cnd;
	int ret = NOTIFY_DONE;
	int i;

	cnd.clk = clk_to_handle(clk);
	cnd.old_rate = old_rate;
	cnd.new_rate = new_rate;

	for (i = 0; i < clk->notifier_count; i++) {
		ret = clk->notifier_call[i](msg, &cnd);
		if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK)
			break;
	}
	return ret;
}

/**
 * __clk_recalc_rates
 * @clk: first clk in the subtree
 * @msg: notification type (see include/linux/clk.h)
 *
 * Walks the subtree of clks starting with clk and recalculates rates as it
 * goes.  Note that if a clk does not implement the .recalc_rate callback then
 * it is assumed that the clock will take on the rate of it's parent.
 *
 * clk_recalc_rates also propagates the POST_RATE_CHANGE notification,
 * if necessary.
 *
 * Caller must hold prepare_lock.
 */
static void __clk_recalc_rates(clk_t *clk, u32 message)
{
	u32 old_rate;
	u32 parent_rate = 0;
	clk_t *child;

	old_rate = clk->rate;

	if (clk->parent)
		parent_rate = clk->parent->rate;

	if (clk->ops->recalc_rate)
		clk->rate = clk->ops->recalc_rate(clk->hw, parent_rate);
	else
		clk->rate = parent_rate;

	if (IS_ERR(clk->rate))
		clk->rate = 0;

	/*
	 * ignore NOTIFY_STOP and NOTIFY_BAD return values for POST_RATE_CHANGE
	 * & ABORT_RATE_CHANGE notifiers
	 */
	if (clk->notifier_count && message)
		__clk_notify(clk, message, old_rate, clk->rate);

	hlist_for_each_entry(child, &clk->children, child_node)
		__clk_recalc_rates(child, message);
}

/**
 * clk_get_rate - return the rate of clk
 * @handle: the clk whose rate is being returned
 *
 * Simply returns the cached rate of the clk, unless CLK_GET_RATE_NOCACHE flag
 * is set, which means a recalc_rate will be issued.
 * If clk is NULL then returns 0.
 */
u32 clk_get_rate(clk_handle_t handle)
{
	u32 rate;
	clk_t *clk = handle_to_clk(handle);
	if (clk == NULL) {
		LOG_E("%s: err input handle:%x", __func__, handle);
		return 0;
	}

	clk_ops_lock();

	if (clk && (get_clk_flag(clk) & CLK_GET_RATE_NOCACHE))
		__clk_recalc_rates(clk, 0);

	rate = __clk_get_rate(clk);
	clk_ops_unlock();

	return rate;
}

/**
 * __clk_speculate_rates
 * @clk: first clk in the subtree
 * @parent_rate: the "future" rate of clk's parent
 *
 * Walks the subtree of clks starting with clk, speculating rates as it
 * goes and firing off PRE_RATE_CHANGE notifications as necessary.
 *
 * Unlike clk_recalc_rates, clk_speculate_rates exists only for sending
 * pre-rate change notifications and returns early if no clks in the
 * subtree have subscribed to the notifications.  Note that if a clk does not
 * implement the .recalc_rate callback then it is assumed that the clock will
 * take on the rate of it's parent.
 *
 * Caller must hold prepare_lock.
 */
static int __clk_speculate_rates(clk_t *clk, u32 parent_rate)
{
	clk_t *child;
	u32 new_rate;
	int ret = NOTIFY_DONE;

	if (clk->ops->recalc_rate)
		new_rate = clk->ops->recalc_rate(clk->hw, parent_rate);
	else
		new_rate = parent_rate;

	/* abort rate change if a driver returns NOTIFY_BAD or NOTIFY_STOP */
	if (clk->notifier_count)
		ret = __clk_notify(clk, PRE_RATE_CHANGE, clk->rate, new_rate);

	if (ret & NOTIFY_STOP_MASK)
		goto out;

	hlist_for_each_entry(child, &clk->children, child_node) {
		ret = __clk_speculate_rates(child, new_rate);
		if (ret & NOTIFY_STOP_MASK)
			break;
	}

out:
	return ret;
}

static void clk_calc_subtree(clk_t *clk, u32 new_rate)
{
	clk_t *child;

	clk->new_rate = new_rate;

	hlist_for_each_entry(child, &clk->children, child_node) {
		if (child->ops->recalc_rate)
			child->new_rate = child->ops->recalc_rate(child->hw, new_rate);
		else
			child->new_rate = new_rate;
		clk_calc_subtree(child, child->new_rate);
	}
}

/*
 * calculate the new rates returning the topmost clock that has to be
 * changed.
 */
static clk_t *clk_calc_new_rates(clk_t *clk, u32 rate)
{
	clk_t *top = clk;
	u32 best_parent_rate = 0;
	u32 new_rate;

	/* sanity */
	if (check_clk(clk))
		return NULL;

	/* save parent rate, if it exists */
	if (clk->parent)
		best_parent_rate = clk->parent->rate;

	/* never propagate up to the parent */
	if (!(get_clk_flag(clk) & CLK_SET_RATE_PARENT)) {
		if (!clk->ops->round_rate) {
			clk->new_rate = clk->rate;
			return NULL;
		}
		new_rate = clk->ops->round_rate(
			clk->hw, rate, &best_parent_rate);
		goto out;
	}

	/* need clk->parent from here on out */
	if (!clk->parent) {
		return NULL;
	}

	if (!clk->ops->round_rate) {
		top = clk_calc_new_rates(clk->parent, rate);
		new_rate = clk->parent->new_rate;
		goto out;
	}

	new_rate = clk->ops->round_rate(clk->hw, rate, &best_parent_rate);
	if (best_parent_rate != clk->parent->rate) {
		top = clk_calc_new_rates(clk->parent, best_parent_rate);
		goto out;
	}

out:
	/*
	 * if rate to set is too small(rate < *prate/max_divs), the round_rate
	 * will return err code. so we should return NULL here, to let
	 * clk_set_rate failed
	 */
	if (IS_ERR(new_rate)) {
		LOG_W("clk %s round_rate failed, rate %d, prate %d",
			__clk_get_name(clk->hw),
			(int)rate, (int)best_parent_rate);
		return NULL;
	}

	clk_calc_subtree(clk, new_rate);

	return top;
}

/*
 * Notify about rate changes in a subtree. Always walk down the whole tree
 * so that in case of an error we can walk down the whole tree again and
 * abort the change.
 */
static clk_t *clk_propagate_rate_change(clk_t *clk, u32 event)
{
	clk_t *child, *fail_clk = NULL;
	int ret = NOTIFY_DONE;

	if (clk->rate == clk->new_rate)
		return NULL;

	if (clk->notifier_count) {
		ret = __clk_notify(clk, event, clk->rate, clk->new_rate);
		if (ret & NOTIFY_STOP_MASK)
			fail_clk = clk;
	}

	hlist_for_each_entry(child, &clk->children, child_node) {
		clk = clk_propagate_rate_change(child, event);
		if (clk)
			fail_clk = clk;
	}

	return fail_clk;
}

/*
 * walk down a subtree and set the new rates notifying the rate
 * change on the way
 */
static void clk_change_rate(clk_t *clk)
{
	clk_t *child;
	u32 old_rate;
	u32 best_parent_rate = 0;

	old_rate = clk->rate;

	if (clk->parent)
		best_parent_rate = clk->parent->rate;

	if (clk->ops->set_rate)
		clk->ops->set_rate(clk->hw, clk->new_rate, best_parent_rate);

	if (clk->ops->recalc_rate)
		clk->rate = clk->ops->recalc_rate(clk->hw, best_parent_rate);
	else
		clk->rate = best_parent_rate;

	if (clk->notifier_count && old_rate != clk->rate)
		__clk_notify(clk, POST_RATE_CHANGE, old_rate, clk->rate);

	hlist_for_each_entry(child, &clk->children, child_node)
		clk_change_rate(child);
}

/**
 * clk_set_rate - specify a new rate for clk
 * @handle: the clk whose rate is being changed
 * @rate: the new rate for clk
 *
 * In the simplest case clk_set_rate will only adjust the rate of clk.
 *
 * Setting the CLK_SET_RATE_PARENT flag allows the rate change operation to
 * propagate up to clk's parent; whether or not this happens depends on the
 * outcome of clk's .round_rate implementation.  If *parent_rate is unchanged
 * after calling .round_rate then upstream parent propagation is ignored.  If
 * *parent_rate comes back with a new rate for clk's parent then we propagate
 * up to clk's parent and set it's rate.  Upward propagation will continue
 * until either a clk does not support the CLK_SET_RATE_PARENT flag or
 * .round_rate stops requesting changes to clk's parent_rate.
 *
 * Rate changes are accomplished via tree traversal that also recalculates the
 * rates for the clocks and fires off POST_RATE_CHANGE notifiers.
 *
 * Returns 0 on success, -EERROR otherwise.
 */
int clk_set_rate(clk_handle_t handle, u32 rate)
{
	clk_t *top, *fail_clk;
	int ret = 0;
	clk_t *clk	= handle_to_clk(handle);

	/* prevent racing with updates to the clock topology */
	clk_ops_lock();

	/* bail early if nothing to do */
	if (rate == clk->rate)
		goto out;

	/* calculate new rates and get the topmost changed clock */
	top = clk_calc_new_rates(clk, rate);
	if (!top) {
		LOG_I("err!\n");
		ret = -EINVAL;
		goto out;
	}

	/* notify that we are about to change rates */
	fail_clk = clk_propagate_rate_change(top, PRE_RATE_CHANGE);
	if (fail_clk) {
		LOG_W("%s: failed to set %s rate\n", __func__,
				__clk_get_name(fail_clk->hw));
		clk_propagate_rate_change(top, ABORT_RATE_CHANGE);
		ret = -EBUSY;
		goto out;
	}

	/* change the rates */
	clk_change_rate(top);

out:
	clk_ops_unlock();
	return ret;
}

/**
 * clk_get_parent - return the parent of a clk
 * @handle: the clk whose parent gets returned
 *
 * Simply returns clk->parent.  Returns NULL if clk is NULL.
 */
clk_handle_t clk_get_parent(clk_handle_t handle)
{
	clk_t *parent;
	clk_t *clk = handle_to_clk(handle);

	clk_ops_lock();
	parent = __clk_get_parent(clk);
	clk_ops_unlock();

	return clk_to_handle(parent);
}

/*
 * .get_parent is mandatory for clocks with multiple possible parents.  It is
 * optional for single-parent clocks.  Always call .get_parent if it is
 * available and WARN if it is missing for multi-parent clocks.
 */
static clk_t *__clk_init_parent(clk_t *clk)
{
	clk_t *ret = NULL;
	u8 index;
	int i;

	if (!clk->num_parents)
		goto out;

	for (i = 0; i < clk->num_parents; i++) {
		index = clk->hw->clk_src[i];
		if (CLK_ID_START <= index && index < CLK_ID_MAX)
			clk->parents[i] = get_clk_from_id(index);
	}

	if (clk->num_parents == 1) {
		ret = clk->parents[0];
		goto out;
	}

	if (!clk->ops->get_parent) {
		LOG_W("%s: multi-parent clocks must implement .get_parent\n",
			__func__);
		goto out;
	};

	index = clk->ops->get_parent(clk->hw);
	ret = clk->parents[index];

out:
	return ret;
}

static void clk_reparent(clk_t *clk, clk_t *new_parent)
{
	hlist_del(&clk->child_node);

	if (new_parent)
		hlist_add_head(&clk->child_node, &new_parent->children);
	else
		hlist_add_head(&clk->child_node, &clk_orphan_list);

	clk->parent = new_parent;
}

static u8 clk_fetch_parent_index(clk_t *clk, clk_t *parent)
{
	u8 i;

	/*
	 * find index of new parent clock using cached parent ptrs,
	 */
	for (i = 0; i < clk->num_parents; i++) {
		if (clk->parents && clk->parents[i] == parent)
			break;
	}

	return i;
}

static int __clk_set_parent(clk_t *clk, clk_t *parent, u8 p_index)
{
	int ret = 0;
	clk_t *old_parent = clk->parent;
	int migrated_enable = RT_FALSE;

	/* migrate enable */
	if (clk->enable_count) {
		__clk_enable(parent);
		migrated_enable = RT_TRUE;
	}

	/* update the clk tree topology */
	clk_reparent(clk, parent);

	/* change clock input source */
	if (parent && clk->ops->set_parent)
		ret = clk->ops->set_parent(clk->hw, p_index);

	if (ret) {
		/*
		 * The error handling is tricky due to that we need to release
		 * the spinlock while issuing the .set_parent callback. This
		 * means the new parent might have been enabled/disabled in
		 * between, which must be considered when doing rollback.
		 */
		clk_reparent(clk, old_parent);

		if (migrated_enable && clk->enable_count) {
			__clk_disable(parent);
		} else if (migrated_enable && (clk->enable_count == 0)) {
			__clk_disable(old_parent);
		} else if (!migrated_enable && clk->enable_count) {
			__clk_disable(parent);
			__clk_enable(old_parent);
		}
		return ret;
	}

	/* clean up enable for old parent if migration was done */
	if (migrated_enable)
		__clk_disable(old_parent);

	return 0;
}

/**
 * clk_set_parent - switch the parent of a mux clk
 * @handle: the mux clk whose input we are switching
 * @phandle: the new input to clk
 *
 * Re-parent clk to use parent as it's new input source.  If clk has the
 * CLK_SET_PARENT_GATE flag set then clk must be gated for this
 * operation to succeed.  After successfully changing clk's parent
 * clk_set_parent will update the clk topology, sysfs topology and
 * propagate rate recalculation via __clk_recalc_rates.  Returns 0 on
 * success, -EERROR otherwise.
 */
int clk_set_parent(clk_handle_t handle, clk_handle_t phandle)
{
	int ret = 0;
	u8 p_index = 0;
	u32 p_rate = 0;
	clk_t *clk = handle_to_clk(handle);
	clk_t *parent = handle_to_clk(phandle);

	if (!clk || !clk->ops)
		return -EINVAL;

	/* verify ops for for multi-parent clks */
	if ((clk->num_parents > 1) && (!clk->ops->set_parent))
		return -ENOSYS;

	/* prevent racing with updates to the clock topology */
	clk_ops_lock();

	if (clk->parent == parent)
		goto out;

	/* try finding the new parent index */
	if (parent) {
		p_index = clk_fetch_parent_index(clk, parent);
		p_rate = parent->rate;
		if (p_index == clk->num_parents) {
			LOG_D("%s: clk %s can not be parent of clk %s\n",
				__func__, __clk_get_name(parent->hw),
				__clk_get_name(clk->hw));
			ret = -EINVAL;
			goto out;
		}
	}

	/* propagate PRE_RATE_CHANGE notifications */
	ret = __clk_speculate_rates(clk, p_rate);

	/* abort if a driver objects */
	if (ret & NOTIFY_STOP_MASK)
		goto out;

	/* do the re-parent */
	ret = __clk_set_parent(clk, parent, p_index);

	/* propagate rate recalculation accordingly */
	if (ret)
		__clk_recalc_rates(clk, ABORT_RATE_CHANGE);
	else
		__clk_recalc_rates(clk, POST_RATE_CHANGE);

out:
	clk_ops_unlock();

	return ret;
}

/**
 * clk_notifier_register - add a clk rate change notifier
 * @handle: clk_t * to watch
 * @notifier_call:  callback function
 */
int clk_notifier_register(clk_handle_t handle, notifier_fn_t notifier_call)
{
	int ret = -ENOMEM;
	clk_t *clk = handle_to_clk(handle);

	if (!clk || !notifier_call)
		return -EINVAL;

	clk_ops_lock();
	if (clk->notifier_count >= CLK_MAX_NOTIFIER) {
		LOG_E("clk(%s) reach max notifer!", __clk_get_name(clk->hw));
		goto out;
	}
	if (clk->notifier_call == NULL) {
		clk->notifier_call =
			rt_malloc(sizeof(notifier_fn_t) * CLK_MAX_NOTIFIER);
		if (clk->notifier_call == NULL)
			goto out;
	}
	clk->notifier_call[clk->notifier_count] = notifier_call;
	clk->notifier_count++;
	ret = 0;
out:
	clk_ops_unlock();

	return ret;
}

/**
 * clk_notifier_unregister - remove a clk rate change notifier
 * @handle: clk_t *
 * @nb: struct notifier_block * with callback info
 *
 * Request no further notification for changes to 'clk' and frees memory
 * allocated in clk_notifier_register.
 *
 * Returns -EINVAL if called with null arguments; otherwise, passes
 * along the return value of srcu_notifier_chain_unregister().
 */
int clk_notifier_unregister(clk_handle_t handle, notifier_fn_t notifier_call)
{
	int ret = -EINVAL;
	int index;
	clk_t *clk = handle_to_clk(handle);

	if (!clk || !notifier_call)
		return -EINVAL;

	clk_ops_lock();
	if ((clk->notifier_call == NULL) || (clk->notifier_count <= 0))
		goto out;
	for (index = 0; index < clk->notifier_count; index++) {
		if (clk->notifier_call[index] == notifier_call)
			break;
	}
	if (index < clk->notifier_count) {
		clk->notifier_count--;
		clk->notifier_call[index] =
			clk->notifier_call[clk->notifier_count];
		clk->notifier_call[clk->notifier_count] = NULL;
		ret = 0;
	}
out:
	clk_ops_unlock();

	return ret;
}

clk_handle_t clk_get(const char *id)
{
	int i;
	clk_t *retval = NULL;
	clk_t *clk_srcs = NULL;

	if (id == NULL)
		return -EINVAL;

	for (i = CLK_ID_START; i < CLK_NUM; i++) {
		clk_srcs = get_clk_from_id(i);
		if (clk_srcs->hw == NULL) {
			LOG_W("clock %d not init\n", i);
			continue;
		}
		if (!rt_strcmp(id, clk_srcs->hw->name)) {
			retval = clk_srcs;
			break;
		}
	}
	return clk_to_handle(retval);
}

void clk_put(clk_handle_t clk)
{
	return;
}

/**
 * __clk_init - initialize the data structures in a clk_t
 * @clk:	clk being initialized
 *
 * Initializes the lists in clk_t, queries the hardware for the
 * parent and rate and sets them both. We do not set clk's rate and call clk->ops->init.
 * we will do it after all clk hw is initialized.
 */
int __clk_init(clk_t *clk)
{
	int ret = 0;
	struct clk_hw *hw;
	if (!clk) {
		LOG_D("%s error!", __func__);
		return -EINVAL;
	}

	hw = clk->hw;
	if (!hw) {
		LOG_D("%s error!", __func__);
		return -EINVAL;
	}

	/* check that clk_ops are sane.  See Documentation/clk.txt */
	if (clk->ops->set_rate &&
			!(clk->ops->round_rate && clk->ops->recalc_rate)) {
		LOG_W("%s: %s must implement .round_rate & .recalc_rate\n",
				__func__, __clk_get_name(hw));
		ret = -EINVAL;
		goto out;
	}

	if (clk->ops->set_parent && !clk->ops->get_parent) {
		LOG_W("%s: %s must implement .get_parent & .set_parent\n",
				__func__, __clk_get_name(hw));
		ret = -EINVAL;
		goto out;
	}

	clk->parent = __clk_init_parent(clk);

	/*
	 * Populate clk->parent if parent has already been __clk_init'd.  If
	 * parent has not yet been __clk_init'd then place clk in the orphan
	 * list.  If clk has set the CLK_IS_ROOT flag then place it in the root
	 * clk list.
	 *
	 * Every time a new clk is clk_init'd then we walk the list of orphan
	 * clocks and re-parent any that are children of the clock currently
	 * being clk_init'd.
	 */
	if (clk->parent)
		hlist_add_head(&clk->child_node,
				&clk->parent->children);
	else if (get_clk_flag(clk) & CLK_IS_ROOT)
		hlist_add_head(&clk->child_node, &clk_root_list);
	else {
		hlist_add_head(&clk->child_node, &clk_orphan_list);
		LOG_E("%s %d: we find a orphan clock:%s\n",
			__func__, __LINE__, __clk_get_name(clk->hw));
	}

out:
	return ret;
}

/**
 * This function will init lombo board
 */
int clk_init(void)
{
	int i;
	clk_t *clk;
	static int init_flag = -1;

	if (init_flag > 0)
		return 0;

	init_flag = 1;
	rt_mutex_init(&clk_lock, "clk_lock", RT_IPC_FLAG_FIFO);
	init_clk_hw();
	memset(g_clk_src, 0, sizeof(g_clk_src));
	for (i = CLK_ID_START; i < CLK_NUM; i++) {
		clk = get_clk_from_id(i);
		clk->hw = &clk_hw_all[i];
		if (clk->hw->name == NULL) {
			LOG_E("clk name is not setting!\n");
			clk->hw->name = "unnamed clk";
		}
		if (clk->hw->flags & CLK_TYPE_FIXED_CLOCK) {
			clk_init_fixed_rate(clk);
			clk->enable_count++;
		}
		if (clk->hw->flags & CLK_TYPE_FACT_CLOCK)
			clk_init_fixed_factor(clk);
		if (clk->hw->flags & CLK_TYPE_GATE_CLOCK)
			clk_init_gate(clk);
		if (clk->hw->flags & CLK_TYPE_DEVIDER_CLOCK)
			clk_init_divider(clk);
		if (clk->hw->flags & CLK_TYPE_PLL_CLOCK)
			clk_init_pll(clk);
		if (clk->hw->flags & CLK_TYPE_MODULE_CLOCK)
			clk_init_module(clk);
		__clk_init(clk);
		if (clk->ops->is_enabled)
			clk->enable_count = clk->ops->is_enabled(clk->hw);
	}

	hlist_for_each_entry(clk, &clk_root_list, child_node)
		__clk_recalc_rates(clk, 0);

	for (i = CLK_ID_START; i < CLK_NUM; i++) {
		clk = get_clk_from_id(i);
		if (clk->ops->init)
			clk->ops->init(clk->hw);
	}

	return 0;
}
/* INIT_BOARD_EXPORT(init_lombo_clk); */

