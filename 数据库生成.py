import sqlite3

conn = sqlite3.connect('virus_database.db')
c = conn.cursor()
virus_md5_values = ['e4d909c290d0fb1ca068ffaddf22cbd0', '827ccb0eea8a706c4c34a16891f84e7b']#去网上找指纹

conn = sqlite3.connect('virus_database.db')
c = conn.cursor()

# 将所有病毒 MD5 值添加到数据库中
for md5_value in virus_md5_values:
    insert_query = """
    INSERT INTO md5_values (value)
    VALUES (?);
    """
    c.execute(insert_query, (md5_value,))

conn.commit()
c.close()
conn.close()

import hashlib
import os
import sqlite3

def scan_directory(directory_path, db_file_path):
    """
    扫描指定目录下的所有文件，并与病毒库进行比较，
    如果发现病毒文件，则打印警告信息。

    Args:
        directory_path: string，要扫描的目录路径。
        db_file_path: string，SQLite3 数据库文件的路径和名称。

    Returns:
        None
    """
    # 连接 SQLite3 数据库
    conn = sqlite3.connect(db_file_path)
    c = conn.cursor()

    # 获取所有病毒 MD5 值
    select_query = """
    SELECT value FROM md5_values;
    """
    c.execute(select_query)
    virus_md5_values = set([row[0] for row in c.fetchall()])
    for foldername, subfolders, filenames in os.walk(directory_path):# 遍历指定目录及子目录下的所有文件
        for filename in filenames:
            file_path = os.path.join(foldername, filename)
            with open(file_path, 'rb') as f:# 对文件进行 MD5 计算
                file_hash = hashlib.md5(f.read()).hexdigest()
            # 比对病毒库中的 MD5 值
            if file_hash in virus_md5_values:
                print("警告：发现病毒文件！", file_path)
                os.chmod(file_path, 0o644)#修改为禁止运行
    # 关闭 SQLite3 连接
    c.close()
    conn.close()
