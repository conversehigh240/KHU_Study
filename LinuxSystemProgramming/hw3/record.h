#define SMAX 20
#define NMAX 10
#define DMAX 15

typedef struct {
    char stud[SMAX];
    char num[NMAX];
    char dept[DMAX];
} Record;

/* 헤더 파일에 구조체를 정의함. 여기서 typedef를 이용하여 구조체에 Record라는
   이름을 부여했기 때문에 struct를 명시하지 않고, Record를 이용하여
   구조체를 사용할 수 있음*/