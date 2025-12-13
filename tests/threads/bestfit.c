#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void test_bestfit (void) 
{
  palloc_set_mode (PAL_BEST_FIT);


  // 추가, 테스트용
  /* 2. 단편화(Fragmentation) 상황 만들기 
     [ A (2칸) ] [ B (1칸-벽) ] [ C (1칸) ] [ D (1칸-벽) ] */
  
  void *a = palloc_get_multiple (0, 2); 
  void *b = palloc_get_page (0);        
  void *c = palloc_get_page (0);        
  void *d = palloc_get_page (0);        

  ASSERT (a != NULL && b != NULL && c != NULL && d != NULL);

  size_t idx_a = palloc_get_page_index (a);
  size_t idx_c = palloc_get_page_index (c);

  /* 구멍 뚫기: 
     - 0번지 쪽에 크기 2짜리 구멍 (A 해제)
     - 뒤쪽에 크기 1짜리 구멍 (C 해제) */
  palloc_free_multiple (a, 2); 
  palloc_free_page (c);        

  /* 3. 테스트 요청 (크기 1짜리 할당) */
  void *fit = palloc_get_page (0);
  
  ASSERT (fit != NULL);
  
  size_t idx_fit = palloc_get_page_index (fit);

  /* [검증 로직]
     - First Fit이었다면? -> 앞에서부터 찾으니 큰 구멍(A자리, idx_a)에 넣었을 것임.
     - Best Fit이라면?    -> 딱 맞는 구멍(C자리, idx_c)을 찾아 넣었을 것임. */

  if (idx_fit == idx_a) {
      PANIC ("FAIL: Behavior matches First Fit (took the larger hole first).");
  }

  if (idx_fit != idx_c) {
      PANIC ("FAIL: Did not pick the best fitting hole.");
  }

  /* 성공 시 아무 메시지 없이 조용히 종료 -> PASS */

  /* 4. 뒷정리 */
  palloc_free_page (b);
  palloc_free_page (d);
  palloc_free_page (fit);
}
