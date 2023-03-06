#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

void md5sum(const char *file_path, unsigned char *digest) {
    FILE *fp = fopen(file_path, "rb");
    if (fp == NULL) {
        printf("无法打开文件: %s", file_path);
        exit(1);
    }
    MD5_CTX c;
    int len;
    unsigned char buffer[1024];
    MD5_Init(&c);
    while ((len = fread(buffer, 1, 1024, fp)) != 0) {
        MD5_Update(&c, buffer, len);
    }
    MD5_Final(digest, &c);
    fclose(fp);
}

void search_and_delete_file(const char *file_path, const char **hash_table, size_t table_size) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    md5sum(file_path, digest);
    char md5_string[MD5_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&md5_string[i * 2], "%02x", (unsigned int) digest[i]);
    }
    for (int i = 0; i < table_size; i++) {
        if (strcmp(md5_string, hash_table[i]) == 0) {
            printf("找到了匹配的哈希值：%s\n", md5_string);
            if (remove(file_path) == 0) {
                printf("成功删除文件：%s\n", file_path);
            } else {
                perror("删除文件失败");
            }
            return;
        }
    }
}

int main() {
    const char *directory_path = "指定目录";
    const char *file_path_to_delete = "指定文件路径";
    const char *hash_table[] = {"e4d909c290d0fb1ca068ffaddf22cbd0",
                                "827ccb0eea8a706c4c34a16891f84e7b"};
    size_t table_size = sizeof(hash_table) / sizeof(hash_table[0]);

    search_and_delete_file(file_path_to_delete, hash_table, table_size);

    return 0;
}
#在主程序中，我们定义要扫描的目录、要搜索和删除的文件路径以及哈希表。然后，调用 search_and_delete_file() 函数来搜索指定的文件路径是否存在于哈希表中。如果存在，则删除该文件并打印消息“成功删除文件：XXXX”，其中“XXXX” 是删除的文件路径。
#请注意，此示例代码使用 OpenSSL 库来计算 MD5 哈希值。如果您的系统上没有安装 OpenSSL，请根据您的操作系统和编译环境选择适当的库或方法来计算哈希值。
