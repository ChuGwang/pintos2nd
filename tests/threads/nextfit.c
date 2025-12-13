#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void test_nextfit (void) 
{
  palloc_set_mode (PAL_NEXT_FIT);


  // 추가, 테스트용이므로 삭제 후 제출
  /* 2. A, B 할당 */
  void *a = palloc_get_page (0);
  void *b = palloc_get_page (0);

  /* 할당 자체가 실패하면 안 되므로 체크 */
  ASSERT (a != NULL);
  ASSERT (b != NULL);
  
  /* 인덱스 확인 (조용히 검사) */
  size_t idx_a = palloc_get_page_index (a);
  size_t idx_b = palloc_get_page_index (b);

  /* 3. A 반납 (0번지를 비움) */
  palloc_free_page (a);

  /* 4. C 할당 (여기가 핵심!) */
  void *c = palloc_get_page (0);
  ASSERT (c != NULL);

  size_t idx_c = palloc_get_page_index (c);

  /* [검증 로직]
     First Fit이라면? -> 빈 0번지(idx_a)에 들어갔을 것임.
     Next Fit이라면?  -> 1번지 뒤인 2번지(idx_b + 1)에 들어갔을 것임. */
  
  /* 만약 C가 A의 자리(0번)로 다시 돌아갔다면 Next Fit이 아니므로 강제 종료(FAIL) */
  if (idx_c == idx_a) {
      PANIC ("FAIL: Output is First Fit behavior, not Next Fit.");
  }

  /* 만약 C가 B의 다음 자리로 잘 갔다면 OK (아무것도 출력 안 함) */
  ASSERT (idx_c > idx_b);

  /* 5. 뒷정리 */
  palloc_free_page (b);
  palloc_free_page (c);
  
}

