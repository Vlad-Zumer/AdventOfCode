import ssl
import os
import os
from urllib.request import urlretrieve

def ensurePackage(importName:str, packageName:str):
    try:
        __import__(importName)
    except:
        os.system("pip install "+packageName)

ensurePackage("yaml", "pyyaml")
import yaml
import json

LANGS_ALL="allLangs.yml"
LANGS_USED="usedLangs.json"

def getLangsYamlFile():
    tmp = ssl._create_default_https_context
    ssl._create_default_https_context = ssl._create_unverified_context
    url = "https://raw.githubusercontent.com/github-linguist/linguist/master/lib/linguist/languages.yml"
    urlretrieve(url, LANGS_ALL)
    ssl._create_default_https_context = tmp

def getLangsUsedJsonFile():
    tmp = ssl._create_default_https_context
    ssl._create_default_https_context = ssl._create_unverified_context
    url = "https://api.github.com/repos/Vlad-Zumer/AdventOfCode/languages"
    urlretrieve(url, LANGS_USED)
    ssl._create_default_https_context = tmp



def removeLangsFiles():
    try:
        os.remove(LANGS_ALL)
    except:
        pass

    try:
        os.remove(LANGS_USED)
    except:
        pass



removeLangsFiles()
getLangsYamlFile()
getLangsUsedJsonFile()

usedLangs = set()
allLangs = []

with open(LANGS_USED) as fstream:
    usedLangsObj:dict = json.load(fstream)
    for langName in usedLangsObj:
        usedLangs.add(langName)

with open(LANGS_ALL, "r") as fstream:
    langs:dict = yaml.safe_load(fstream)
    langName:str
    langData:dict
    for langName, langData in langs.items():
        if langData.get("type", None) == "programming":
            allLangs.append(langName)

allLangs.sort()

allLangsMDList:list[str] = []
usedLangsMDList:list[str] = []
for lang in allLangs:
    if lang in usedLangs:
        usedLangsMDList.append("<li> " + lang + "\n")
        allLangsMDList.append("* [x] " + lang + "\n")
    else:
        allLangsMDList.append("* [ ] " + lang + "\n")

fileContent:list[str]= []
with open("README.md", "r") as fstream:
    fileContent = fstream.readlines()

newFileContent:str = ""
inUsedLangsList:bool = False
indent:str = ""
for line in fileContent:
    if line.strip() == "<!-- UsedLangListStart -->":
        inUsedLangsList = True
        indent = " " * (len(line) - len(line.lstrip()))
    if line.strip() == "<!-- UsedLangListEnd -->":
        inUsedLangsList = False
        for usedLangLine in usedLangsMDList:
            newFileContent += indent + usedLangLine

    if inUsedLangsList and line.strip().startswith("<li>"):
        continue

    newFileContent += line

with open("README.md", "w") as fstream:
    fstream.write(newFileContent)

with open("language_table.md", "w") as fstream:
    fstream.write(''.join(allLangsMDList))

removeLangsFiles()