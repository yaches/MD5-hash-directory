#include <iostream>
#include <fstream>
#include <string>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <openssl/md5.h>

using namespace std;

#define FILE_BUFFER_SIZE 1024

void print_md5_sum(unsigned char* md) {
	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		printf("%02x", md[i]);
	}
	printf("\n");
}

void MD5_file(string filename, unsigned char* hash) {
	unsigned char buffer[FILE_BUFFER_SIZE];

	FILE* file = fopen(filename.c_str(), "rb");

	if (!file) {
		cout << "file err: " << filename << endl;
		return;
	}

	MD5_CTX mdContext;
	MD5_Init(&mdContext);
	
	int reading_bytes = 0;

	do {
		reading_bytes = fread(buffer, 1, FILE_BUFFER_SIZE, file);
		MD5_Update (&mdContext, buffer, reading_bytes);
	} while (reading_bytes > 0);
	
	MD5_Final (hash, &mdContext);

	fclose(file);
}

void MD5_dir(string dir_name) {

	DIR* dir = opendir(dir_name.c_str());
	struct dirent* d;
	struct stat file_info;

	unsigned char hash[MD5_DIGEST_LENGTH];

	if (!dir) {
		exit(1);
		cout << "dir err" << endl;
	}

	string file_relative_name;

	while ((d = readdir(dir)) != NULL) {
		if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
			file_relative_name = dir_name + "/" + string(d->d_name);
			lstat(file_relative_name.c_str(), &file_info);
			if (S_ISDIR(file_info.st_mode)) {
				MD5_dir(file_relative_name);
			} else {
				MD5_file(file_relative_name, hash);
				cout << file_relative_name << ": ";
				print_md5_sum(hash);
			}
		}
	}

	closedir(dir);
}

int main(int argc, char const *argv[])
{
	unsigned char result[MD5_DIGEST_LENGTH];

	string dir_name;
	cin >> dir_name;

	MD5_dir(dir_name);

	// MD5_file(filename, result);
	// print_md5_sum(result);

	return 0;
}