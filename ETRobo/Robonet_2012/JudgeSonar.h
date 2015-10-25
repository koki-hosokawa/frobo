#ifndef JUDGESONAR_H_
#define JUDGESONAR_H_
/*************************************************************************

  JudgeSonar.h

*************************************************************************/
/*===========================================================================
  include
===========================================================================*/
#include "ecrobot_interface.h"
#include "SonarData.h"


/*===========================================================================
  define
===========================================================================*/

/*===========================================================================
  尻尾判定クラス
===========================================================================*/
/* 属性 */
typedef struct {
/*	JudgeMethod inherit_judgeMethod;	判定方法クラス(スーパークラス) */
	SonarData* sonarData;
} JudgeSonar;

/* 操作 */
void JudgeSonar_init(JudgeSonar* this, SonarData* sonarData);				/* 初期化操作 */
void JudgeSonar_finish(JudgeSonar* this);						/* 終了操作 */
S8 JudgeSonar_execution(JudgeSonar* this, S16, S16);			/* 実行 */

#endif

