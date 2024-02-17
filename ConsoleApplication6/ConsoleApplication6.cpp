#include <filesystem>
#include <string>
#include <iostream>
#include <thread>         

const char* kextensionPng = ".png";
const char* kextensionJpg = ".jpg";

const short int KMore_This_Kilobytes = 100;

namespace fs = std::filesystem;

class Initial_Files_Copied {
public:
    Initial_Files_Copied(const fs::path& sourceDirectory, const fs::path& destinationDirectory) {
        Copy_File_Content(sourceDirectory, destinationDirectory);
    }

    static void Copy_File_Content(const fs::path& sourceDirectory, const fs::path& destinationDirectory);
};
class DeviceInitialFilesCopier {
private:
    const fs::path& DeviceDrive;

public:
    DeviceInitialFilesCopier(const fs::path& sourceDirectory, const fs::path& destinationDirectory)
        :DeviceDrive(destinationDirectory) {
        if (fs::exists(sourceDirectory) && fs::exists(destinationDirectory)) { 
            Copy_Files_To_Destination_Directory(sourceDirectory);
        }
    }

private:
    void Copy_Files_To_Destination_Directory(const fs::path& current_path);
};

//namespace X

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();
    const char* sourceDirectory = "C:\\Users";
    const char* destinationDirectory = "C:\\Users\\rolde\\Downloads\\Copy File";
    DeviceInitialFilesCopier deviceInitialFiles(sourceDirectory, destinationDirectory);

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedTime = endTime - startTime;
    std::cout << std::endl << elapsedTime.count();
    return 0;
}

void Initial_Files_Copied::Copy_File_Content(const fs::path& sourceDirectory, const fs::path& destinationDirectory) {
    try {
        fs::copy_file(sourceDirectory, destinationDirectory, fs::copy_options::overwrite_existing);
    }
    catch (const fs::filesystem_error) {}
}

void DeviceInitialFilesCopier::Copy_Files_To_Destination_Directory(const fs::path& current_path) {
    for (const auto& file : fs::directory_iterator(current_path)) {
        try {
            if (fs::is_directory(file)) { Copy_Files_To_Destination_Directory(file); }
        }
        catch (const std::system_error&) {}
        if (fs::file_size(file) > KMore_This_Kilobytes * 0x400) {
            if (file.path().extension() == kextensionPng) {
                std::thread t(&Initial_Files_Copied::Copy_File_Content, file, this->DeviceDrive / file.path().filename());
                t.join();
            }
        }
    }
}
