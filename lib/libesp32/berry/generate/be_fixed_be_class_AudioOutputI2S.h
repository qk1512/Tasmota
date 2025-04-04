#include "be_constobj.h"

static be_define_const_map_slots(be_class_AudioOutputI2S_map) {
    { be_const_key_weak(set_rate, -1), be_const_ctype_func(be_audio_output_set_rate) },
    { be_const_key_weak(set_bits_per_sample, -1), be_const_ctype_func(be_audio_output_set_bits_per_sample) },
    { be_const_key_weak(flush, -1), be_const_ctype_func(be_audio_output_i2s_flush) },
    { be_const_key_weak(begin, -1), be_const_ctype_func(be_audio_output_i2s_begin) },
    { be_const_key_weak(consume_silence, -1), be_const_ctype_func(be_audio_output_consume_silence) },
    { be_const_key_weak(set_channels, 8), be_const_ctype_func(be_audio_output_set_channels) },
    { be_const_key_weak(init, 5), be_const_ctype_func(be_audio_output_i2s_init) },
    { be_const_key_weak(_X2Ep, -1), be_const_var(0) },
    { be_const_key_weak(stop, -1), be_const_ctype_func(be_audio_output_i2s_stop) },
    { be_const_key_weak(consume_mono, 2), be_const_ctype_func(be_audio_output_consume_mono) },
    { be_const_key_weak(consume_stereo, -1), be_const_ctype_func(be_audio_output_consume_stereo) },
    { be_const_key_weak(set_gain, 7), be_const_ctype_func(be_audio_output_set_gain) },
    { be_const_key_weak(deinit, -1), be_const_ctype_func(be_audio_output_i2s_deinit) },
};

static be_define_const_map(
    be_class_AudioOutputI2S_map,
    13
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_AudioOutputI2S,
    1,
    NULL,
    AudioOutputI2S
);
