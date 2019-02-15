set(qwt_HDRS
    qwt.h
    qwt_abstract_scale_draw.h
    qwt_clipper.h
    qwt_color_map.h
    qwt_compat.h
    qwt_column_symbol.h
    qwt_date.h
    qwt_date_scale_draw.h
    qwt_date_scale_engine.h
    qwt_dyngrid_layout.h
    qwt_global.h
    qwt_graphic.h
    qwt_interval.h
    qwt_interval_symbol.h
    qwt_math.h
    qwt_magnifier.h
    qwt_null_paintdevice.h
    qwt_painter.h
    qwt_painter_command.h
    qwt_panner.h
    qwt_picker.h
    qwt_picker_machine.h
    qwt_pixel_matrix.h
    qwt_point_3d.h
    qwt_point_polar.h
    qwt_round_scale_draw.h
    qwt_scale_div.h
    qwt_scale_draw.h
    qwt_scale_engine.h
    qwt_scale_map.h
    qwt_spline.h
    qwt_symbol.h
    qwt_system_clock.h
    qwt_text_engine.h
    qwt_text_label.h
    qwt_text.h
    qwt_transform.h
    qwt_widget_overlay.h
)

# add QwtWidgets
set(qwt_HDRS
    ${qwt_HDRS}
    qwt_abstract_slider.h
    qwt_abstract_scale.h
    qwt_arrow_button.h
    qwt_analog_clock.h
    qwt_compass.h
    qwt_compass_rose.h
    qwt_counter.h
    qwt_dial.h
    qwt_dial_needle.h
    qwt_knob.h
    qwt_slider.h
    qwt_thermo.h
    qwt_wheel.h
)

set(qwt_SRCS
    qwt_abstract_scale_draw.cpp
    qwt_clipper.cpp
    qwt_color_map.cpp
    qwt_column_symbol.cpp
    qwt_date.cpp
    qwt_date_scale_draw.cpp
    qwt_date_scale_engine.cpp
    qwt_dyngrid_layout.cpp
    qwt_event_pattern.cpp
    qwt_graphic.cpp
    qwt_interval.cpp
    qwt_interval_symbol.cpp
    qwt_math.cpp
    qwt_magnifier.cpp
    qwt_null_paintdevice.cpp
    qwt_painter.cpp
    qwt_painter_command.cpp
    qwt_panner.cpp
    qwt_picker.cpp
    qwt_picker_machine.cpp
    qwt_pixel_matrix.cpp
    qwt_point_3d.cpp
    qwt_point_polar.cpp
    qwt_round_scale_draw.cpp
    qwt_scale_div.cpp
    qwt_scale_draw.cpp
    qwt_scale_map.cpp
    qwt_spline.cpp
    qwt_scale_engine.cpp
    qwt_symbol.cpp
    qwt_system_clock.cpp
    qwt_text_engine.cpp
    qwt_text_label.cpp
    qwt_text.cpp
    qwt_transform.cpp
    qwt_widget_overlay.cpp
)

# add QwtWidgets
set(qwt_SRCS
    ${qwt_SRCS}
    qwt_abstract_slider.cpp
    qwt_abstract_scale.cpp
    qwt_arrow_button.cpp
    qwt_analog_clock.cpp
    qwt_compass.cpp
    qwt_compass_rose.cpp
    qwt_counter.cpp
    qwt_dial.cpp
    qwt_dial_needle.cpp
    qwt_knob.cpp
    qwt_slider.cpp
    qwt_thermo.cpp
    qwt_wheel.cpp
)

set(qwt_FORMS
)

qt5_wrap_cpp(qwt_HDRS_MOC ${qwt_HDRS})
qt5_wrap_ui(qwt_FORM_HDRS ${qwt_FORMS})

set(CMAKE_DEBUG_POSTFIX d)

add_library(qwt SHARED ${qwt_SRCS} ${qwt_HDRS_MOC} ${qwt_FORM_HDRS})

target_link_libraries(
    qwt
    Qt5::Widgets
)
