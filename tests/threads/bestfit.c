#include "tests/threads/tests.h"
#include "threads/palloc.h"
#include <stdio.h>

void test_bestfit (void) 
{
  palloc_set_mode (PAL_BEST_FIT);


  // 추가, 테스트용
  /* 2. 상황 만들기: 메모리에 구멍 뚫기 (Fragmentation)
     목표: [ 큰 구멍 (2칸) ] --- [ 작은 구멍 (1칸) ] 상태 만들기 */
  
  // A: 0~1번지 (2페이지) 할당
  void *a = palloc_get_multiple (0, 2); 
  // B: 2번지 (1페이지) 할당 (칸막이 역할)
  void *b = palloc_get_page (0);        
  // C: 3번지 (1페이지) 할당
  void *c = palloc_get_page (0);        
  // D: 4번지 (1페이지) 할당 (칸막이 역할)
  void *d = palloc_get_page (0);        

  /* 구멍 뚫기 */
  palloc_free_multiple (a, 2); // 0번지에 '크기 2'짜리 큰 구멍 생김
  palloc_free_page (c);        // 3번지에 '크기 1'짜리 딱 맞는 구멍 생김

  /* 3. Best Fit 테스트 요청 (1페이지 필요)
     - First Fit이라면? "어? 0번지 비었네?" 하고 0번에 넣음.
     - Best Fit이라면? "0번은 너무 넓고, 3번이 딱 맞네!" 하고 3번에 넣음. */
  void *fit = palloc_get_page (0);

  if (fit != NULL) {
    int idx = palloc_get_page_index (fit);
    msg ("Allocated at index %d", idx);
    
    // (참고용) 결과를 해석해주는 메시지
    if (idx == 3)
      msg ("SUCCESS: Detected best fit correctly.");
    else if (idx == 0)
      msg ("FAIL: It acts like First Fit.");
    else
      msg ("FAIL: Unexpected index.");
  } 
  else {
    msg ("Failed to allocate");
  }

  /* 4. 뒷정리 */
  palloc_free_page (b);
  palloc_free_page (d);
  if (fit != NULL) palloc_free_page (fit);
}
