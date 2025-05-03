#include <stdio.h>
#include <Windows.h>
#include <time.h>

double cpu() {
	FILETIME idle, kernel, user;//GetSystemTime 에 받기 위해 선언
	int i = 0;
	double r = 0;
	//idle kernel user를 이렇게 가져온다
	GetSystemTimes(&idle, &kernel, &user);

	//kernel user idle의 첫번째 high 와 low를 구한다
	DWORD kernel_high1 = kernel.dwHighDateTime;
	DWORD kernel_low1 = kernel.dwLowDateTime;
	DWORD user_high1 = user.dwHighDateTime;
	DWORD user_low1 = user.dwLowDateTime;
	DWORD idle_high1 = idle.dwHighDateTime;
	DWORD idle_low1 = idle.dwLowDateTime;

	//시간이 0.5 이다
	Sleep(500);

	//kernel user idle의 두번째 high와 low를 구한다(가져오는거 포함)
	GetSystemTimes(&idle, &kernel, &user);
	DWORD kernel_high2 = kernel.dwHighDateTime;
	DWORD kernel_low2 = kernel.dwLowDateTime;
	DWORD user_high2 = user.dwHighDateTime;
	DWORD user_low2 = user.dwLowDateTime;
	DWORD idle_high2 = idle.dwHighDateTime;
	DWORD idle_low2 = idle.dwLowDateTime;

	//두 시점을 구해서 서로 빼준다.
	//윈도우 특성이 시간을 64-> 32비트로 나눠서 표현하기에 이 작업을 한다
	//아래와 같이 정확한 차를 구한다.
	//시간이 0.1*5 = 0.5가 된다

	while (i < 5) {
		double kernel = (kernel_high2 + kernel_low2) - (kernel_high1 + kernel_low1);
		double user = (user_high2 + user_low2) - (user_high1 + user_low1);
		double idle = (idle_high2 + idle_low2) - (idle_high1 + idle_low1);
		// 제시되었던 load 구하기 식을 쓴다
		double res = (user + kernel - idle) * 100 / (user + kernel);

		Sleep(100);
		r = r + res;
		i++;
	}
	// 둘이 합쳐서 1초의 시간동안 이를 수행하게 된다.
	return r / 5;
}
int main() {
	int n = 0;
	// 총 1000초 동안 받을수 있다.
	double a[1000];
	SYSTEMTIME t;//localTime 함수 받는 부분
	SYSTEM_INFO info;//GetSystemInfo 함수의 매개변수
	GetSystemInfo(&info);
	//위에 있던 cpu의 개수를 위 함수를 통해 구한다
	printf("# of CPU's : %d\n", info.dwNumberOfProcessors);

	while (1) {
		GetLocalTime(&t);
		double cpu_load = cpu();
		// 반복하면서 배열에 넣는다
		a[n] = cpu_load;

		if (n == 0) {
			// 첫번쨰 줄이 공백, 따라서 공백 구현
			printf("%d %d.%02d.%02d %02d:%02d:%02d :", n, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		if (n >= 1) {
			//매초마다 작동
			printf("%d %d.%02d.%02d %02d:%02d:%02d : [CPU Load: %0.2f%%]", n, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, cpu_load);
		}
		if (n < 5) {
			printf("\n");
		}
		else if (n >= 5 && n < 10) {
			printf(" [5sec avg]: %0.2f%%\n", (double)(a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 5);
		}
		else if (n < 15) {
			printf(" [5sec avg]: %0.2f%% [10sec avg]: %0.2f%%\n",
				(double)(a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 5,
				(double)(a[n - 9] + a[n - 8] + a[n - 7] + a[n - 6] + a[n - 5] + a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 10);
		}
		else if (n >= 15) {
			printf(" [5sec avg]: %0.2f%% [10sec avg]: %0.2f%% [15sec avg] : %0.2f%%\n",
				(double)(a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 5,
				(double)(a[n - 9] + a[n - 8] + a[n - 7] + a[n - 6] + a[n - 5] + a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 10,
				(double)(a[n - 14] + a[n - 13] + a[n - 12] + a[n - 11] + a[n - 10] + a[n - 9] + a[n - 8] + a[n - 7] + a[n - 6] + a[n - 5] + a[n - 4] + a[n - 3] + a[n - 2] + a[n - 1] + a[n]) / 15);

		}
		n++;
		//배열을 통해 특정 구간동안의 평균을 구한다.
		//저장된 인덱스에서 - 하는 형태로 구했다(이전 시간).
	}
	return 0;
}