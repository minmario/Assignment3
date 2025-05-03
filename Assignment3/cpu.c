#include <stdio.h>
#include <Windows.h>
#include <time.h>

double cpu() {
	FILETIME idle, kernel, user;//GetSystemTime �� �ޱ� ���� ����
	int i = 0;
	double r = 0;
	//idle kernel user�� �̷��� �����´�
	GetSystemTimes(&idle, &kernel, &user);

	//kernel user idle�� ù��° high �� low�� ���Ѵ�
	DWORD kernel_high1 = kernel.dwHighDateTime;
	DWORD kernel_low1 = kernel.dwLowDateTime;
	DWORD user_high1 = user.dwHighDateTime;
	DWORD user_low1 = user.dwLowDateTime;
	DWORD idle_high1 = idle.dwHighDateTime;
	DWORD idle_low1 = idle.dwLowDateTime;

	//�ð��� 0.5 �̴�
	Sleep(500);

	//kernel user idle�� �ι�° high�� low�� ���Ѵ�(�������°� ����)
	GetSystemTimes(&idle, &kernel, &user);
	DWORD kernel_high2 = kernel.dwHighDateTime;
	DWORD kernel_low2 = kernel.dwLowDateTime;
	DWORD user_high2 = user.dwHighDateTime;
	DWORD user_low2 = user.dwLowDateTime;
	DWORD idle_high2 = idle.dwHighDateTime;
	DWORD idle_low2 = idle.dwLowDateTime;

	//�� ������ ���ؼ� ���� ���ش�.
	//������ Ư���� �ð��� 64-> 32��Ʈ�� ������ ǥ���ϱ⿡ �� �۾��� �Ѵ�
	//�Ʒ��� ���� ��Ȯ�� ���� ���Ѵ�.
	//�ð��� 0.1*5 = 0.5�� �ȴ�

	while (i < 5) {
		double kernel = (kernel_high2 + kernel_low2) - (kernel_high1 + kernel_low1);
		double user = (user_high2 + user_low2) - (user_high1 + user_low1);
		double idle = (idle_high2 + idle_low2) - (idle_high1 + idle_low1);
		// ���õǾ��� load ���ϱ� ���� ����
		double res = (user + kernel - idle) * 100 / (user + kernel);

		Sleep(100);
		r = r + res;
		i++;
	}
	// ���� ���ļ� 1���� �ð����� �̸� �����ϰ� �ȴ�.
	return r / 5;
}
int main() {
	int n = 0;
	// �� 1000�� ���� ������ �ִ�.
	double a[1000];
	SYSTEMTIME t;//localTime �Լ� �޴� �κ�
	SYSTEM_INFO info;//GetSystemInfo �Լ��� �Ű�����
	GetSystemInfo(&info);
	//���� �ִ� cpu�� ������ �� �Լ��� ���� ���Ѵ�
	printf("# of CPU's : %d\n", info.dwNumberOfProcessors);

	while (1) {
		GetLocalTime(&t);
		double cpu_load = cpu();
		// �ݺ��ϸ鼭 �迭�� �ִ´�
		a[n] = cpu_load;

		if (n == 0) {
			// ù���� ���� ����, ���� ���� ����
			printf("%d %d.%02d.%02d %02d:%02d:%02d :", n, t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
		}
		if (n >= 1) {
			//���ʸ��� �۵�
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
		//�迭�� ���� Ư�� ���������� ����� ���Ѵ�.
		//����� �ε������� - �ϴ� ���·� ���ߴ�(���� �ð�).
	}
	return 0;
}