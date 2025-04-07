#include "audio_config.h"

/* XXX: make sure to update the copies in the different encoders if you change
* this table */
static const int avpriv_mpeg4audio_sample_rates[13] = {
	96000, 88200, 64000, 48000, 44100, 32000,
	24000, 22050, 16000, 12000, 11025, 8000, 7350
};

/*static const uint8_t ff_mpeg4audio_channels[8] = {
	0, 1, 2, 3, 4, 5, 6, 8
};*/

int put_audio_specific_config(unsigned char *pb, int len, ga_config_data *data)
{
	int i = 0;
	int ret = 0;
	int channels = data->numChannels - (data->numChannels == 8 ? 1 : 0);

	if (len < 8)
		return 0;

	/* Samplerate */
	for (i = 0; i < 13; i++)
		if (data->samplingFrequency == avpriv_mpeg4audio_sample_rates[i])
			break;

	pb[0] = data->audioObjectType << 3;/* 5bits */

	if (i == 13) {
		pb[0] |= 0x07;
		pb[1] = 0x80;
		pb[1] |= (data->samplingFrequency >> (24 - 7)) & 0x7f;
		pb[2] = (data->samplingFrequency >> (24 - 15)) & 0xff;
		pb[3] = (data->samplingFrequency >> (24 - 23)) & 0xff;
		pb[4] = (data->samplingFrequency & 0x01) << 7;
		pb[4] |= (channels & 0xf) << 3;
		/* GASpecificConfig 3bits = 0
		Explicitly Mark SBR absent
		11bits = 0x2b7 */
		pb[2 + 3] = 0x2b7 >> (11 - 8);
		pb[3 + 3] = (0x2b7 & 0x07) << 5;
		/* 5bits = AOT_SBR = 5 */
		pb[3 + 3] |= 5;
		pb[4 + 3] = 0;
		ret = 5 + 3;
	} else {
		pb[0] |= i >> 1;
		pb[1] |= i << 7;
		pb[1] |= (channels & 0xf) << 3;
		/* GASpecificConfig 3bits = 0
		Explicitly Mark SBR absent
		11bits = 0x2b7 */
		pb[2] = 0x2b7 >> (11 - 8);
		pb[3] = (0x2b7 & 0x07) << 5;
		/* 5bits = AOT_SBR = 5 */
		pb[3] |= 5;
		pb[4] = 0;
		ret = 5;
	}

	return ret;
}
