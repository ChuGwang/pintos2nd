#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void
test_firstfit (void) 
{
  palloc_set_mode (PAL_FIRST_FIT);

  // 테스트 위해 추가
  /* 2. 메모리 페이지 할당 요청 (테스트용) */
  void *a = palloc_get_page (0);
  void *b = palloc_get_page (0);
  
  /* 3. 할당된 결과 출력 (이 메시지가 .ck 파일과 일치해야 함) */
  if (a != NULL)
    msg ("Allocated A at index %d", palloc_get_page_index (a));
  else
    msg ("Failed to allocate A");

  if (b != NULL)
    msg ("Allocated B at index %d", palloc_get_page_index (b));
  else
    msg ("Failed to allocate B");

  /* 4. 메모리 해제 (뒷정리) */
  palloc_free_page (a);
  palloc_free_page (b);

}
