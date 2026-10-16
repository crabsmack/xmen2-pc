/* Spark_ClassInit - per-level class setup of SPARK (class slot +$5E): only calls the empty
 * Spark_NopStatic (g_SparkSprDef is loaded by the scenes). The parent's isn't called. */
#include "xmen2.h"

void Spark_ClassInit(void)
{
	Spark_NopStatic();
}
