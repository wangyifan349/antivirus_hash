use std::collections::HashMap;
use std::fs::File;
use std::io::Read;
use std::path::PathBuf;
use std::process::Command;





fn search_and_delete_file(file_path: &PathBuf, hash_table: &HashMap<String, PathBuf>) -> bool {
    let mut file = File::open(file_path).unwrap();
    let mut contents = Vec::new();
    file.read_to_end(&mut contents).unwrap();
    let file_hash = format!("{:x}", md5::compute(&contents));
    if hash_table.contains_key(&file_hash) {
        println!("找到了匹配的哈希值：{}", file_hash);
        Command::new("rm").arg(file_path).output().unwrap();
        println!("成功删除文件：{:?}", file_path);
        true
    } else {
        false
    }
}

fn main() {
    let directory_path = String::from("指定目录");
    let file_path_to_delete = PathBuf::from("指定文件路径");

    let mut hash_table: HashMap<String, PathBuf> = HashMap::new();

    for entry in walkdir::WalkDir::new(&directory_path).into_iter().filter_map(|e| e.ok()) {
        if let Ok(mut file) = File::open(entry.path()) {
            let mut contents = Vec::new();
            if file.read_to_end(&mut contents).is_ok() {
                let file_hash = format!("{:x}", md5::compute(&contents));
                hash_table.insert(file_hash, entry.path().to_owned());
            }
        }
    }

    search_and_delete_file(&file_path_to_delete, &hash_table);
}
