add_library(dsp)

target_include_directories(dsp PUBLIC common/include)
target_include_directories(dsp PUBLIC common/include_sim)
target_include_directories(dsp PUBLIC fft/include)

target_sources(dsp PUBLIC
        common/misc/dsps_pwroftwo.cpp
        fft/float/dsps_fft2r_fc32_ansi.c
        fft/float/dsps_fft4r_fc32_ansi.c
        fft/float/dsps_fft2r_bitrev_tables_fc32.c
        fft/float/dsps_fft4r_bitrev_tables_fc32.c
)