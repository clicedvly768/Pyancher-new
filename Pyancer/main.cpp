
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <thread>

class MinecraftLauncher {
private:
    std::string username;
    std::string version;
    std::string gameDir;

public:
    MinecraftLauncher() : gameDir(".minecraft") {}

    void setUsername(const std::string& name) {
        username = name;
    }

    void setVersion(const std::string& ver) {
        version = ver;
    }

    void setGameDirectory(const std::string& dir) {
        gameDir = dir;
    }

    bool downloadGameFiles() {
        std::cout << "Downloading Minecraft " << version << "..." << std::endl;
        return true;
    }

    bool launchGame() {
        std::string command = "java -jar " + gameDir + "/versions/" + version + "/" + version + ".jar" +
            " --username " + username +
            " --gameDir " + gameDir;

        std::cout << "Launching Minecraft with command: " << command << std::endl;

        int result = system(command.c_str());
        return result == 0;
    }

    void showMenu() {
        std::cout << "=== Minecraft Launcher ===" << std::endl;
        std::cout << "1. Set username" << std::endl;
        std::cout << "2. Set version" << std::endl;
        std::cout << "3. Launch game" << std::endl;
        std::cout << "4. Exit" << std::endl;
    }
};

int main() {
    MinecraftLauncher launcher;
    int choice;

    do {
        launcher.showMenu();
        std::cout << "Choose option: ";
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::string username;
            std::cout << "Enter username: ";
            std::cin >> username;
            launcher.setUsername(username);
            break;
        }
        case 2: {
            std::string version;
            std::cout << "Enter version (e.g., 1.20.1): ";
            std::cin >> version;
            launcher.setVersion(version);
            break;
        }
        case 3: {
            if (launcher.downloadGameFiles()) {
                launcher.launchGame();
            }
            break;
        }
        case 4:
            std::cout << "Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid option!" << std::endl;
        }
    } while (choice != 4);

    return 0;
}