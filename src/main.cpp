#include <raylib.h>
#include <wb/utils.h>

int main() {
	int myInt = 0;
	for (int i=0;i<11;i++) {
		myInt+=i;
	}
	PRINT_DBG(myInt);
    return 0;
}
