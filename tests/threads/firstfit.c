#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void
test_firstfit (void) 
{
  palloc_set_mode (PAL_FIRST_FIT);

  /* 2. 페이지 두 개 연속 할당 */
  void *a = palloc_get_page (0);
  void *b = palloc_get_page (0);

  /* [검증] 할당이 성공해야 함 */
  ASSERT (a != NULL);
  ASSERT (b != NULL);

  size_t idx_a = palloc_get_page_index (a);
  size_t idx_b = palloc_get_page_index (b);

  /* [검증] First Fit이라면 B는 A 바로 다음 위치여야 함 (순차 할당) */
  if (idx_b != idx_a + 1) {
      PANIC ("FAIL: First fit did not allocate sequentially.");
  }

  /* 3. 뒷정리 (성공 시 조용히 종료 -> PASS) */
  palloc_free_page (a);
  palloc_free_page (b);
}
