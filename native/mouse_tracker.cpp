#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

// Send a JSON message to Firefox
void sendNativeMessage(const string& json) {
    uint32_t len = json.length();
    fwrite(&len, 4, 1, stdout);
    fwrite(json.c_str(), 1, len, stdout);
    fflush(stdout);
}

// Keep native messaging connection alive
void inputListener() {
    while (true) {
        uint32_t len;
        if (!fread(&len, 4, 1, stdin)) break;

        string input(len, '\0');
        if (!fread(&input[0], 1, len, stdin)) break;

        cerr << "Received from Firefox/Chrome (optional): " << input << endl;
    }
}

int main() {
    const int EDGE_THRESHOLD = 1;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);

    // Thread to listen to input from browser (to keep native connection open)
    thread readerThread(inputListener);

    // Cooldown timers for each edge
    auto lastLeftSwitch = chrono::steady_clock::now() - chrono::milliseconds(500);
    auto lastRightSwitch = chrono::steady_clock::now() - chrono::milliseconds(500);

    while (true) {
        POINT p;
        if (GetCursorPos(&p)) {
            auto now = chrono::steady_clock::now();

            if (p.x <= EDGE_THRESHOLD &&
                chrono::duration_cast<chrono::milliseconds>(now - lastLeftSwitch).count() > 500) {
                sendNativeMessage(R"({"action":"switch-left"})");
                lastLeftSwitch = now;
            }
            else if (p.x >= screenWidth - EDGE_THRESHOLD &&
                chrono::duration_cast<chrono::milliseconds>(now - lastRightSwitch).count() > 500) {
                sendNativeMessage(R"({"action":"switch-right"})");
                lastRightSwitch = now;
            }
        }

        this_thread::sleep_for(chrono::milliseconds(20));
    }

    readerThread.join();
    return 0;
}
