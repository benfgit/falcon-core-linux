
#include <g3log/loglevels.hpp>

// all values with a + 1 higher than their closest equivalent
// they could really have the same value as well.


const LEVELS STATE {INFO.value + 1, {"STATE"}};
const LEVELS EVENT {STATE.value +1, {"EVENT"}};
const LEVELS UPDATE {EVENT.value + 1, {"UPDATE"}};
const LEVELS ERROR {WARNING.value + 1, {"ERROR"}};

