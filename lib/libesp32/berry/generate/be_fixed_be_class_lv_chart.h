#include "be_constobj.h"

static be_define_const_map_slots(be_class_lv_chart_map) {
    { be_const_key_weak(_class, -1), be_const_comptr(&lv_chart_class) },
    { be_const_key_weak(init, -1), be_const_func(be_ntv_lv_chart_init) },
};

static be_define_const_map(
    be_class_lv_chart_map,
    2
);

BE_EXPORT_VARIABLE be_define_const_class(
    be_class_lv_chart,
    0,
    (bclass *)&be_class_lv_obj,
    lv_chart
);
