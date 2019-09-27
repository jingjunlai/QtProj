#ifndef MATHTOOLS
#define MATHTOOLS

#include <QtMath>

#ifndef GET_BIT
#define GET_BIT(XX, n) ((XX & (1<<(n))) >> (n))
#endif

#ifndef SET_BET
#define SET_BIT(XX, n) (XX |= (1<<(n)))
#endif

#ifndef CLR_BET
#define CLR_BIT(XX, n) (XX &= ~(1<<(n)))
#endif

#ifndef GET_FLOOR
#define GET_FLOOR(value, base)  qFloor((double)(value) / (base)) * (base)
#endif

#ifndef GET_CEIL
#define GET_CEIL(value, base)   qCeil((double)(value) / (base)) * (base)
#endif

#endif // MATHTOOLS

