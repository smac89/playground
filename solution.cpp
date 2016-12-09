#include <iostream>
#include <cstdlib>

int main() {
	
    // Remove synchronization between cstdio and iostream
    std::ios_base::sync_with_stdio(false);
    // Prevent flushing of output stream before each io operation
    std::cin.tie(nullptr);

    return EXIT_SUCCESS;
}
