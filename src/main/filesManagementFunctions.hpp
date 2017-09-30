//
// Created by Santos on 11/27/2016.
//

#ifndef CPSWITHSPLINES_FILESMANAGEMENTEFUNCTIONS_H
#define CPSWITHSPLINES_FILESMANAGEMENTEFUNCTIONS_H

#include "main.hpp"

typedef struct image_for_experiments {
    std::string imageClass;
    std::string imageName;
    cv::Mat image;
} gImage;

std::vector<cv::Mat> readImagesFromDirectory(std::string directoryFullPath);
gImage readImageOnPath(std::string imageFullPath);

std::vector<std::string> getClassDirectories(std::string directoryFullPath);
std::vector<std::string> getImageDirectories(std::string directoryFullPath);
std::string getClassFromPath(std::string fullPath);
std::string getNameFromPath(std::string fullPath);

std::string getNameFromPath(std::string fullPath){

    std::stringstream ss(fullPath);
    std::string token, lastToken;
    char delim = '\\';

    while(getline(ss, token, delim)) {
        lastToken = token;
    }

    return lastToken;
}

std::string getClassFromPath(std::string fullPath){

    std::stringstream ss(fullPath);
    std::string token, lastToken = "-1", previousToLastToken = "-2";
    char delim = '\\';

    while(getline(ss, token, delim)) {
        previousToLastToken = lastToken;
        lastToken = token;
    }

    return previousToLastToken;
}

std::vector<std::string> getImageDirectories(std::string directoryFullPath) {
    DIR *dir;
    struct dirent *imageFile;

    std::vector<std::string> allDirectories;

    if ((dir = opendir(directoryFullPath.c_str())) != NULL) {
        while ((imageFile = readdir(dir)) != NULL) {
            std::string imageFileName = std::string(imageFile->d_name, imageFile->d_namlen);
            if (imageFileName.compare(".") != 0 && imageFileName.compare("..") != 0) {
                std::string imagePath = directoryFullPath;
                imagePath += std::string(imageFile->d_name, imageFile->d_namlen);
                allDirectories.push_back(imagePath);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        perror("");
    }

    return allDirectories;
}

gImage readImageOnPath(std::string imagePath) {

    gImage theImage;

    theImage.image = cv::imread(imagePath,0);
    theImage.imageClass = getClassFromPath(imagePath);
    theImage.imageName = getNameFromPath(imagePath);

    return theImage;
}

std::vector<cv::Mat> readImagesFromDirectory(std::string directoryFullPath) {
    DIR *dir;
    struct dirent *imageFile;

    std::vector<cv::Mat> allImages;

    if ((dir = opendir(directoryFullPath.c_str())) != NULL) {
        while ((imageFile = readdir(dir)) != NULL) {
            std::string imageFileName = std::string(imageFile->d_name, imageFile->d_namlen);
            if (imageFileName.compare(".") != 0 && imageFileName.compare("..") != 0) {
                std::string imagePath = directoryFullPath;
                imagePath += std::string(imageFile->d_name, imageFile->d_namlen);


                cv::Mat currentImage = cv::imread(imagePath, 0);
                allImages.push_back(currentImage);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        perror("");
    }

    return allImages;
}

std::vector<std::string> getClassDirectories(std::string directoryFullPath) {
    DIR *dir;
    struct dirent *classFolder;

    std::vector<std::string> allClasses;

    if ((dir = opendir(directoryFullPath.c_str())) != NULL) {
        while ((classFolder = readdir(dir)) != NULL) {
            std::string classFolderName = std::string(classFolder->d_name, classFolder->d_namlen);
            if (classFolder->d_type == DT_DIR && classFolderName.compare(".") != 0 && classFolderName.compare("..") != 0) {
                std::string classPath = directoryFullPath;
                classPath += std::string(classFolder->d_name, classFolder->d_namlen);
                classPath += "\\";

                allClasses.push_back(classPath);
            }
        }
        closedir(dir);
    } else {
        /* could not open directory */
        perror("");
    }

    return allClasses;
}

#endif //CPSWITHSPLINES_FILESMANAGEMENTEFUNCTIONS_H
