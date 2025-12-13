#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void test_nextfit (void) 
{
  palloc_set_mode (PAL_NEXT_FIT);


  // 추가, 테스트용이므로 삭제 후 제출
  /* 2. A 할당 (Index 0 예상) */
  void *a = palloc_get_page (0);
  if (a != NULL)
    msg ("Allocated A at index %d", palloc_get_page_index (a));

  /* 3. B 할당 (Index 1 예상) 
     -> 여기까지 하면 마지막 탐색 위치(scan_idx)는 2가 되어야 함 */
  void *b = palloc_get_page (0);
  if (b != NULL)
    msg ("Allocated B at index %d", palloc_get_page_index (b));

  /* 4. A를 해제 (Index 0이 비어있는 상태가 됨) */
  palloc_free_page (a);
  msg ("Freed A");

  /* 5. C 할당 (테스트의 핵심!)
     - First Fit이라면? 0번이 비었으니 0번에 할당함.
     - Next Fit이라면? 아까 1번 뒤(2번)를 기억하고 있으니 2번에 할당함. */
  void *c = palloc_get_page (0);
  if (c != NULL) {
    int idx = palloc_get_page_index (c);
    msg ("Allocated C at index %d", idx);

    /* (참고) 검증 메시지 */
    if (idx > palloc_get_page_index(b))
        msg ("SUCCESS: Maintained next fit property.");
    else
        msg ("FAIL: Returned to the beginning (First Fit behavior).");
  }

  /* 6. 뒷정리 */
  palloc_free_page (b);
  if (c != NULL) palloc_free_page (c);
  
}

