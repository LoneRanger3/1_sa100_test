#ifndef AUDIO_CONFIG_H
#define AUDIO_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */
#include <oscl.h>

enum AudioObjectType {
	AACNULL,
	AACMAIN = 1,
	AACLC = 2,
	AACSSR = 3,
	AACLTP = 4,
	AACSBR = 5,
	AACSCALABLE = 6,
	TWINVQ = 7
};

/**
* Make AAC audio config object.
* @see 1.6.2.1 "Syntax - AudioSpecificConfig"
*/
typedef struct ga_config_data_struct {
	uint32_t audioObjectType;
	uint32_t samplingFrequency;
	uint32_t numChannels;
} ga_config_data;

int put_audio_specific_config(unsigned char *pb, int len, ga_config_data *data);

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef AUDIO_CONFIG_H */
