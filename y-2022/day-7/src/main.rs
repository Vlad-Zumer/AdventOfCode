#![allow(non_snake_case)]
use std::{fs::File, io::{BufReader, BufRead}, collections::HashMap};

#[derive(Debug, Clone)]
struct MockFile
{
    name: String,
    size: u64,
}

#[derive(Debug, Clone)]
struct MockDir
{  
    name: String,
    subDirs: HashMap<String, MockDir>,
    files: HashMap<String, MockFile>
}

impl MockDir {

    fn new<S>(name: S) -> Self
    where S: Into<String>
    {
        return MockDir
        {
            name: name.into(),
            subDirs: HashMap::new(),
            files: HashMap::new(),
        };
    }

    fn getSize(&self, recursive: bool) -> u64
    {
        let mut size = self.files.values().map(|f| f.size).sum();

        if recursive && self.subDirs.len() > 0
        {
            size += self.subDirs.values().map(|dir| dir.getSize(recursive)).sum::<u64>()
        }

        return size;
    }

    fn getSubDirs(&self, recursive: bool) -> Vec<&MockDir>
    {
        let mut dirs = Vec::new();
        
        for sd in self.subDirs.values()
        {
            dirs.push(sd);
        }

        if recursive && self.subDirs.len() > 0
        {
            for recSd in  self.subDirs.values().flat_map(|sdir| sdir.getSubDirs(recursive))
            {
                dirs.push(recSd);
            }                                          
        }

        return  dirs;
    }
}

struct MockFileSystem
{}

impl MockFileSystem
{
    fn getFlatStructure(fsTree: &MockDir) -> Vec<(String, &MockDir)>
    {
        let mut ret: Vec<(String, &MockDir)> = fsTree.getSubDirs(false)
            .into_iter()
            .flat_map(|sdir| MockFileSystem::getFlatStructure(sdir))
            .map(|(elName, elSDir)| (format!("{}/{}", fsTree.name.clone(), elName), elSDir))
            .collect();
    
        ret.push((fsTree.name.clone(), fsTree));

        return ret;
    }

    fn fixAbsolutePath<S>(path: S) -> String
    where S: Into<String>
    {
        let mut thisPath: String = path.into();
        assert!(thisPath.starts_with("/"), "Path is not absolute");

        // root path
        if thisPath.eq("/")
        {
            return  thisPath;
        }

        thisPath = thisPath.trim_matches('/').to_string();

        // ".." processing needed
        if thisPath.contains("..")
        {

            // ".." processing
            let mut skips = 0;
            let mut finalPath = Vec::new();
            for part in thisPath.split("/").collect::<Vec<_>>().into_iter().rev()
            {
                if part == ".."
                {
                    skips += 1;
                    continue;
                }

                if skips > 0
                {
                    skips -= 1;
                    continue;
                }

                finalPath.push(part)
            }

            finalPath.reverse();
            thisPath = finalPath.join("/");
        }

        // Fix start of path
        while !thisPath.starts_with("//")
        {
            thisPath.insert(0, '/');
        }

        // Fix end of path
        while thisPath.ends_with("/") 
        {
            thisPath.pop();
        }

        return  thisPath;
    }

    fn mkdir<S>(parentDir: &mut MockDir, newDirName: S)
    where S: Into<String>
    {
        let mut path: String = newDirName.into();
        path = path.trim_matches('/').to_string();
        assert!(!path.contains('/'));

        parentDir.subDirs.insert(path.clone(), MockDir::new(path));
    }

    fn mkdir_rec<S>(parentDir: &mut MockDir, newDirsPath: S)
    where S: Into<String>
    {
        let mut path: String = newDirsPath.into();
        path = path.trim_matches('/').to_string();

        let mut currDir = parentDir;
        for name in path.split("/")
        {
            assert!(!currDir.subDirs.contains_key(name));
            currDir.subDirs.insert(name.to_string(), MockDir::new(name.to_string()));
            currDir = currDir.subDirs.get_mut(name).unwrap();
        }
    }

    fn mkdir_abs_rec<S>(fsRoot: &mut MockDir, absPath: S)
    where S: Into<String>
    {
        let mut path: String = absPath.into();
        path = MockFileSystem::fixAbsolutePath(path);

        // remove root + trailing '/'
        path = path.trim_start_matches('/').to_string();

        // process path
        let mut curr = fsRoot;
        let pathParts = path.split('/').collect::<Vec<_>>();
        let mut skip = 0;

        for dir in pathParts.clone()
        {
            if !curr.subDirs.contains_key(dir)
            {
                break;
            }

            curr = curr.subDirs.get_mut(dir).unwrap();
            skip += 1;
        }

        let remainingPath = pathParts.into_iter().skip(skip).fold(String::new(), |acc, el| acc + "/" + el);

        MockFileSystem::mkdir_rec(curr, remainingPath);

    }

    fn getDir<S>(fsRoot: &mut MockDir, absPath: S) -> Option<&mut MockDir>
    where S: Into<String>
    {
        let mut path: String = absPath.into();
        path = MockFileSystem::fixAbsolutePath(path);

        if path.eq("/")
        {
            return Some(fsRoot);
        }

        // remove root + trailing '/'
        path = path.trim_start_matches('/').to_string();

        // process path
        let mut curr = fsRoot;
        let pathParts = path.split('/').collect::<Vec<_>>();

        for dir in pathParts.clone()
        {
            if !curr.subDirs.contains_key(dir)
            {
                return None;
            }

            curr = curr.subDirs.get_mut(dir).unwrap();
        }

        return Some(curr);
    }
    
    fn touch<S>(parentDir: &mut MockDir, fileName: S, size: u64)
    where S: Into<String>
    {
        let name: String = fileName.into();
        parentDir.files.insert(name.clone(), MockFile { name, size });
    }

}

fn parseInput(filePath : &str) -> Option<MockDir>
{
   let f = File::open(filePath).ok()?;
   let lines : Vec<_> = BufReader::new(f).lines().collect();

    if lines.iter().any(|l| l.is_err())
    {
        return None;
    }

    let mut currPath = "/".to_owned();
    let mut root = MockDir::new("/");

    for line in lines.into_iter().skip(1).map(|lr| lr.unwrap())
    {
        if line.starts_with("$")
        {
            // command -> CD or LS
            //  ignore LS, deal with it below
            //  do CD
            let words: Vec<_> = line.split(" ").collect();

            if words[1] != "cd"
            {
                continue;
            }

            if words[2] == "/"
            {
                currPath = "/".to_owned();
            
            }
            else if words[2] == ".."
            {
                currPath += "/";
                currPath += words[2];
                currPath = MockFileSystem::fixAbsolutePath(currPath);
            }
            else
            {    
                currPath += "/";
                currPath += words[2];
            }

        }
        else if line.starts_with("dir")
        {
            // dirs in current path
            let words: Vec<_> = line.split(" ").collect();
            let dirPath = currPath.clone() + "/" + words[1];
            MockFileSystem::mkdir_abs_rec(&mut root, dirPath);
        }
        else 
        {
            // file in current path
            let words: Vec<_> = line.split(" ").collect();
            let dir = MockFileSystem::getDir(&mut root, &currPath).unwrap();
            MockFileSystem::touch(dir, words[1], words[0].parse::<u64>().unwrap());
        }
    }

    return Some(root);
}

fn part1(root: &mut MockDir) -> u64
{
    let mut v = vec![root.getSize(true)];
    v.extend(root.getSubDirs(true).into_iter().map(|sd| sd.getSize(true)));
    return v.into_iter().filter(|size| *size <= 100000).sum();
}

fn part2(root: &mut MockDir) -> (u64, String)
{
    let totalSpace: u64 = 70000000;
    let reqSpace: u64 = 30000000;
    let rootSpace: u64 = root.getSize(true);

    let mut v = vec![(root.getSize(true), root.name.clone())];
    v.extend(root.getSubDirs(true).into_iter().map(|sd| (sd.getSize(true), sd.name.clone())));

    let mut res = v[0].clone();

    for (size, name) in v[1..].into_iter()
    {
        if (totalSpace - (rootSpace - size) >= reqSpace) && (size < &res.0)
        {
            res = (size.clone(), name.clone());
        }
    }

    return  res;

}

fn main() {
    let mut root = parseInput("input.txt").unwrap();

    println!("P1: {:?}", part1(&mut root));
    println!("P2: {:?}", part2(&mut root));
}