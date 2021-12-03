#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<assert.h>

using namespace std;

#pragma region Input

vector<string> inputRules = 
{"97: 138 57 | 12 83",
"131: 20 83 | 74 57",
"7: 57 110 | 83 51",
"48: 17 83 | 56 57",
"2: 83 57",
"40: 57 101 | 83 93",
"16: 12 83 | 47 57",
"42: 15 83 | 66 57",
"62: 83 134 | 57 18",
"55: 124 57 | 45 83",
"1: 57 116 | 83 93",
"63: 83 1 | 57 32",
"83: a",
"72: 57 83 | 83 83",
"18: 83 57 | 57 83",
"67: 90 83 | 99 57",
"91: 83 108 | 57 72",
"8: 42",
"116: 83 18 | 57 72",
"41: 57 83",
"130: 57 84 | 83 50",
"26: 57 12 | 83 124",
"103: 134 57 | 45 83",
"39: 83 18 | 57 138",
"46: 83 47 | 57 138",
"129: 57 138 | 83 108",
"58: 108 57 | 72 83",
"138: 57 57 | 83 57",
"49: 58 57 | 26 83",
"133: 57 79 | 83 13",
"125: 83 124 | 57 72",
"123: 105 83 | 134 57",
"128: 45 57 | 2 83",
"24: 83 117 | 57 76",
"86: 57 12 | 83 85",
"76: 107 57 | 123 83",
"70: 83 37 | 57 112",
"51: 83 45 | 57 54",
"37: 47 57 | 72 83",
"9: 57 124 | 83 105",
"61: 57 19 | 83 47",
"75: 108 83 | 105 57",
"120: 83 105 | 57 18",
"124: 57 57 | 83 83",
"4: 41 83 | 12 57",
"0: 8 11",
"27: 83 41",
"89: 57 29 | 83 40",
"111: 54 83 | 18 57",
"12: 57 57",
"53: 57 52 | 83 73",
"90: 83 130 | 57 48",
"59: 55 83 | 125 57",
"21: 45 83 | 19 57",
"30: 83 7 | 57 70",
"88: 41 57 | 2 83",
"71: 60 57 | 44 83",
"74: 98 57 | 14 83",
"87: 131 57 | 30 83",
"73: 49 83 | 94 57",
"82: 83 41 | 57 138",
"35: 83 22 | 57 89",
"136: 57 105 | 83 108",
"94: 83 129 | 57 86",
"65: 83 72 | 57 108",
"105: 95 95",
"77: 83 78 | 57 92",
"19: 83 83",
"92: 95 138",
"104: 18 83 | 124 57",
"43: 83 102 | 57 103",
"99: 115 57 | 23 83",
"135: 83 93 | 57 97",
"23: 81 57 | 69 83",
"115: 83 132 | 57 25",
"110: 85 57 | 18 83",
"52: 83 3 | 57 137",
"3: 68 57 | 106 83",
"15: 35 83 | 53 57",
"106: 41 83 | 85 57",
"60: 113 83 | 24 57",
"93: 12 57",
"66: 87 83 | 133 57",
"34: 55 83 | 28 57",
"80: 83 41 | 57 85",
"28: 72 57 | 124 83",
"45: 83 57 | 83 83",
"38: 58 57 | 61 83",
"25: 4 83 | 62 57",
"14: 57 124 | 83 138",
"29: 136 83 | 121 57",
"64: 18 83 | 19 57",
"81: 122 57 | 9 83",
"137: 33 83 | 91 57",
"118: 100 57 | 120 83",
"98: 108 57 | 41 83",
"114: 83 72 | 57 45",
"132: 128 57 | 75 83",
"107: 83 134 | 57 72",
"50: 123 83 | 114 57",
"78: 41 83 | 19 57",
"79: 57 118 | 83 77",
"32: 62 57 | 109 83",
"10: 59 83 | 43 57",
"119: 39 57 | 111 83",
"36: 57 134 | 83 41",
"6: 83 2 | 57 138",
"69: 126 83 | 27 57",
"57: b",
"113: 5 83 | 38 57",
"112: 138 83 | 105 57",
"33: 108 83 | 45 57",
"127: 41 57 | 12 83",
"22: 83 34 | 57 119",
"20: 57 127 | 83 80",
"85: 95 57 | 57 83",
"68: 134 57 | 19 83",
"44: 83 63 | 57 10",
"47: 83 95 | 57 57",
"54: 57 57 | 57 83",
"96: 36 57 | 21 83",
"84: 98 57 | 46 83",
"117: 83 16 | 57 65",
"95: 83 | 57",
"122: 18 83 | 41 57",
"101: 83 41 | 57 72",
"121: 134 57 | 108 83",
"56: 82 83 | 6 57",
"17: 83 88 | 57 93",
"108: 57 95 | 83 83",
"109: 134 83 | 124 57",
"134: 95 83 | 83 57",
"5: 104 83 | 64 57",
"100: 41 83 | 138 57",
"126: 54 83 | 45 57",
"11: 42 31",
"13: 57 135 | 83 96",
"31: 83 71 | 57 67",
"102: 19 57 | 72 83",};
vector<string> inputLines = 
{"babbaaaabbbbbbabaaaaabbb",
"bbaababbbbabbaaabaabbabbbbbabbbabbbbbaba",
"baaabbababaabbbabbbbbaab",
"bbbaabbbbbbbaaababbabbabaabaababaaababbabbbababa",
"bababababababbbbabaaaaaababbabaababbabaaabbaabbb",
"bbbbaaababbaababaaabaaba",
"bbbabaabaabbbbabbbbabbba",
"aaaaabbaabbbabababbbaaaabbbbaabaabbbabbbabbbabaababbaabb",
"bababbabaaabbbabbbabaabaaaaaabaaaaaaaaaabbbbbabbaaaaabaaaaaabaab",
"aaaababaaabbbbbabbaabbaaaaaababbabaaaaabbaaababb",
"ababbbbaaabbaaaababaaaaabaabaabaabbbbabbaabaaaaaaababbaababaabaabaabbaaaabababbb",
"aababbaabaabbabababbbabb",
"abaaabaabaabbaaaaababaab",
"abbbbbaababbbabababbabab",
"abaababbbaabaaaaaababbbaabaaabbaababbaaabaabaabaaababbabbabbbbabaababbbbaabaabba",
"bbababbabaaabbababaabbaabaaaabab",
"aabbaaabbbbbbabaaaabbbbbbaabbbbb",
"abbaababbbabbabbbbbaaabababababaabbababaabbbabaaababbbab",
"ababbaaaababbbbbaaaaabbabbabbabbabbbbabbbbbaabaa",
"baaabaabbaaabbabbabbbbba",
"aabbbabaabbbaabbbbabbbbbaabbbabaaaabbbabbbababbaaaaaabaa",
"ababbbababbaabaabaaaaaba",
"baabaabbaababbaabaabbbabbbaaabbb",
"abbabbabaaaabbaaabbbbbbbbbbbaaabbaaaabbbbbaaaabaaaaabbbb",
"bbbbbaaaaabbbbbaababaaba",
"bbaabbaabaabbaabbabababb",
"aaaabbaabbabaabbbabaaaaa",
"abbbbbbaabbbbababbaabbba",
"aaaaabaaabbaaaabaabbabaabbbbbabaabbbabbb",
"abaabbbbaababbbaaababbbbaaaabaaaabaabaaabbbaaaab",
"bbbaaaabaaaababaababbbabbabbababaabbababbabbbbaabbbaaaaabbbbbabaaaabbbaa",
"bbabababaabbaababaaabbbb",
"aaaaaabbaabbbbaaabaaaaabaabbaaaabaabbaaabababbba",
"babaaaabababbaaabbabaaaaaabaabbbbbabaaababaabbbabaabbaaa",
"bbaabaaaaaaaaabbaaaabbaabbbabbbabaaabbbb",
"baabbbaabbaababaaabbbbabbaababaa",
"bbbbbaaaaabbbbababbaabaababbbbbababbbaaaaaaabaaabbaababbbbbabbab",
"babbaaaabbbabbaaabbababb",
"bbbaaabaaabbbbbaababaaaabaabaaab",
"aaaaaabbbbaababaaaaaaaabbaaababa",
"abaabbaabbabbabbbbbbabbaabbbbbbababbabbaaabaaaba",
"abbbababbabaaababaaaabaaabbbabba",
"babaabbbbbababbaaababbbaababaaaa",
"bbbaabbbbbbabaabbbbaaabb",
"aababbbbabbaababbbbaaababbaaaaaa",
"aabbbaaaababbbbabbaaaababbbbaaaa",
"bbababbababababaabaaaabb",
"aaabaabaabbababbbaababaaabbaaaababbbabaabbabbbabbbbbbabbbabaaabb",
"aababbaaaaaaabaaaabababb",
"bbbbbbbbbabaabbbaaaababbabbbaabaabaabbaaaababbabbbaabbbbbbbbbaba",
"bbaaaabbabbbbaabbbabaaabbbbaabbabaaaabbbaabaabba",
"aaaabaaabababaabbbaabaaabaaaababbaababab",
"abbbababaaaabaaababaaaaaaaabbaaaaabaaabbbbbbabaabbbbaaaaaababaabaabbbabb",
"aaabbababbbaabaababbabba",
"aabbabbbaaabbabaabbabaabbaaaababbbbbbabbabababbaabaaabba",
"aabaababaaaaaabaaabbbaaaaabbabaaababbbbaaabaaabbabbbabbaabbabbba",
"abbabaaabaabbaabaababbbaaabbaaaabbbbbabb",
"aabbbaabababbaabbbaaaabaabaaababbbbaaabb",
"bbbabaabababbbbbbbbaabbbaaaabbbb",
"babbaabbaabaaababaaabbabbbabbabaabaabbbabbbbbabbbaaaabbabaaaaaaabbbabbba",
"aababbabaaaabbabaaaababbbababbbbbaabbbbaaabaaaabbaaaaabaaaabbbabbaaaabababaabbaabbbbabaaaabababb",
"abbbbabbbbaababaaaabbabb",
"bbaabbaaaababbabbaaabaababbbaaaaabaaabbaaaaaabbbbbaabbbb",
"abbbbabbbaabbababaaaabba",
"aaaaaabaabbbbababbbaaabb",
"abaabbbabbaaabaabbbbaabababaabba",
"aaababbaababbbbbbabbbbbbbaaabbbb",
"bbbabaaaaabaabbaaabbbbabaaababbbaabbaaabbaababaa",
"aabbbaaaaaabbabaabbbbbab",
"baaaaaabaaabbaabbbabbabbabbabaabbbaabbab",
"abbabaaabbbaabababababbb",
"bbaaaaaabbbbaaaaaabbabab",
"abbbbbaaaaaaaabaabbbbabbbbbbbbbbbbabbbbbabaabaab",
"ababababbbaababbaabbaaabbbaabbaabaaabababbabaabb",
"aaaaaaaababababaaabaaaaa",
"babbbaaabbaaaabbaaaabbba",
"abbbbaabbbbaabbbbabaaaabbabababababaabbbaaabbbbb",
"bababbbabbbbaaaabbaaabbabaaaabbaababbbaa",
"aabaababbbaaabaaaaaabaabbaababba",
"babaaaabbaabbaabbabaabba",
"bbabbababbbaababbbabbbbaaabbabab",
"bbaaaabaaabbabaabbabaaaaabbaabbb",
"aabbabbbbabbbbabbbaaaabbbaabbbbbaabbabba",
"babbabbbaaaaaaaabaabaaab",
"abbbbbbbaaaabbbbaaaabbbababaaaaa",
"aababbabbbbbbaababbbbbab",
"aabbabaaaaabaabbaaaabbaaaabaaaaaabaabbab",
"ababababaabbbbababbbabaaabbbbabbbbabbbbaababbaabbbaaaaaaabbbaaaa",
"abaabababbbbabbbaaabbbab",
"baababbbaaababaabababaab",
"abbbaababbbbaabbaaababab",
"aabaabaaaabaabaababbbbaaabaaaabaaabababbbaaaabab",
"bbbbaabaabaabaaabaaaabaabbabbaabbabaabbabaaaaabbabaabaab",
"aaaabaababaababbabaaaaaabbababaabaaabbaa",
"babababaaaaabaabbbbbabaaabaababbabaaaaab",
"abababaabbabbabbbbbaaabb",
"bbbabbbbabbbaabbbbaaabbbbbbbaaaabbaaaabbabbaababaabaabaa",
"abbbaaaabbbbaaabbabbabbbbabbbbabbaaaaabb",
"bbabaaababbbababbbaabaaa",
"bbaaabaababbaaaabbbabbabaaababaabababaab",
"babababbbabaaabaaabbbabaababbbbb",
"bbbbbbbbbababbbbaaaabaaabbababba",
"aaaaaabbbbbbbbbbababbbaa",
"bbaabababbaababbbaaabbbb",
"bbbbbaaaabbaaaabbbaaabab",
"aaaabaaaabaabbbababababaaaabbbaa",
"abbabbabaaabbababbbabbba",
"aaaabababaaaababaabaaaba",
"aaababbaababaaababbabbabbbaaabaaabbbbaaabaaababbbaaaaaba",
"aaaaabaaabababbabbbbbbbbbbaaaabbabaabbababaaabab",
"abbbbbbaabbbbabbbabababb",
"babbaaaaabaaababaabbbabbbaabaaaa",
"bababbbabbabaaababbaaabb",
"ababababbbaaaabaabbaabbbabbbabaa",
"bbbbaabaaababbabbbaabbab",
"aabbaabababaaabababbbbaaababaaba",
"aaaababaaaaaabaabaababab",
"bbabaabbbbbaabaababbabab",
"babaabbbbbbaabbaaabbbabb",
"babbbababaaabaabaababbabaaababaa",
"aaaabaaabbaaaabababaabaababaabab",
"aaaababbbbbaaabaaabbabbbbaaabaabbbaabbbb",
"abababaabaabbabaababababbabbabaa",
"baaabaabaaababbaaabababa",
"abababaaabbabaabbbbaaaab",
"abbbbaabaaaabbaabaababbbbbaabaabaabbbaababaabbbbabaaaaaaabaabaabbbbabbaabaabbabbaaabaabbabaaaaba",
"bababbbbabababaabbbbbabb",
"aaaaabaaabbbbbbabababababaababbb",
"aabaababbbbaababaababbaaabbbbabbaabbbaba",
"abbbababaaaabbbbbbbbbaababaabaaababababaabbababaaaabaaab",
"aaaabaabaaabaabbbbabbabbaabaaaabbabbbbba",
"bbbabbabbbbaabbbbbbababa",
"aaabaaaaaababbbaabaaaaba",
"abaaaaabbaaaabaaabaaaabb",
"baabbababbaababbbbaabbaababbaaaabaaaababaaabbbaa",
"bbbbaabbaaabaaaabbababaa",
"abbaababbbbbbbbbbbbbabaabaababab",
"abaabbabbaabbbbbababaaba",
"abbbbbaabbbbaababbaabbaabaabbaaaaabbabab",
"babbaaabbbaababbbbbbabbbbaabbabbababaaba",
"aabbbbabaaabbabaabbbaaab",
"bbabbabaabbbbabbabbababb",
"ababbaabaaababbaaabbaababbbabaaabbaaabbbabaabbbbbaababaabbaaabbbaabbabab",
"abbbaaaabbabbabbabbababa",
"bbbbbaabbbbbabbaaabaaaab",
"bbabbaaaaaaabaabbabbaaaaaaaabbabaabbabba",
"baabaabbaabbbbaaaabbbabaaabaabaa",
"abbabaabbaabbabaaaaabbbabaaaaabb",
"ababbbbabababababbbaaabbbabbbbbabaaababbbbababbababaaabb",
"bbbabaababbbbbbbaaababbaabbabbaabbabbabbabbbabbb",
"ababbbbbaabbabbbbaabaaababbabbbbaabaaaaaabbbbbaa",
"aaabaabbabaabaababbaabbbabaabbbababbaababbbbabbabaabaabbbabbbabbaabababbaaaaabbbbbbaabbb",
"baaaaaabbbbaababababbbab",
"baaaaaababbbababaaabbaab",
"ababbababbbaabababbbabba",
"babbaaaaaaabaabbbaabbabaaabbbaba",
"babaaaaabbabaaaabaabbbbbbbbbabbb",
"bbbaaaaaababbaababaaabaa",
"abaabbbabbabaaababbabbabbaabbabb",
"bbbabbabababbaaaabbaaaababaabaaa",
"bbbababbaaaaabaabbbbabaaabbbbbbbabbaaaabababaabaabbbabaa",
"bbaaabbaaabaabbbbaabbbbaababbbaababbbaaa",
"aababbbbabababaaaabaaaab",
"aabbaabaabbbaabaabbaaaabbbbbbaababbbbbbaabaaaabb",
"bbbaabaabbabbbbababaababaabaabba",
"abbbaababbbaabbabaababab",
"aababbaababbabbbaabababa",
"bbbaababaaaabaabbababababbbbaaabbbbabaababaabbab",
"babaaaabaabbbbbaabbbbbaaaaaaaabbbbbaabbaaababaabbabbaaab",
"aaaabaabaaabbabaababbaaabbabbbaabbbabbbaaaabbbbbaabbabba",
"babbbaaaaabbbaabaaaaaabaaaaaabbaabaaabbbaabaaaba",
"aabbbaaabbabababaabbabaaaaabbaaababbabaa",
"bbaaabbabbbabaaaabbaabaaabaaabba",
"baabbbababbababbbaaabababaabaababbaaababbabbbbaa",
"aabaababbbaaaabbbaabbabababaaabaabaabaaa",
"aaaabaaaabbaababaabbbbaa",
"bbbbababbabaabbababbaababbbbbbbaabbaabbabbaabbab",
"abbbbbaabbbbaabaabbbbbab",
"bbaabbaabbaaabbaabaaaaaa",
"aaaababbbbbbbaabaabaabba",
"bbaaaaaabbbabbabbaabaaaa",
"bbbababbaaaaaaabaabbabbabaabaaaaaabaaaba",
"bbaabaabbaaabbbaaaabaaab",
"bbababaabbbabbabaaababbbbbabaabbaaaaabaaaaaababa",
"bbaababaabbaabaaabababbbbaaabbbbbaaaaabbbbabababbabbabbaaaaaaaba",
"bbabababbabaaaababbababa",
"bbabaabbbaaabbbaaaababab",
"abbbaababbaaaababaaabbabaabaaababbbbbbba",
"ababbbbaaaaababaaaaabbba",
"ababbbbbaabbbaaaaabbabba",
"bbabbabbabbaaaabbbbababa",
"abbabbbbbbabaaaaaababbbbababbababbaaabaa",
"bbbbbbbbaabbabaabbabbbab",
"bbababbabaabaabbabbaababbabbbbbbababaaaababaabaa",
"aaaababbaaabbbbaaabbaabb",
"baaabbabbababaaabaabaaba",
"baaaabbaabbaaaaabaaabbaa",
"aabbabaaabbbbabaababbaaabbabbbbababbabab",
"baaaaaabaabbbaabaababbaaaababbbbbbaaababbabbaababababbab",
"babaaaabbabaabababaaabbabbabbbababaabbbb",
"baabbabaaababbbaababbabb",
"aaaaaabababbbaabbaaabaabbaababaaabaaaaababbbbaaaabbbaaba",
"bbaaaabbbbabaaaabbbbbabb",
"abaabbaabbabbbbbbaaabbbabbabaaaabbbaaabbbbaabbab",
"ababaaaababbbbbabaabaaababbbaabbabaaaabbbaabababaaababaababbbbaabaababaabbbabaaa",
"babbbabaabbabaabbbaabbba",
"ababababbbabbabababbaaab",
"abababababababbabbababbbbababaabababbbabaaabbaabababaaaa",
"bbbabbaabababaaabaaabaabaabbaababababbaa",
"abbbababaaaabaaaabbabaaabaaaabbbabbbabbaaabaaaaaaaabababbaaaaaaa",
"bababaaaaaababbababbabaa",
"aaababbabbaaaabbabaaabab",
"bbbabbabbaabbbbaaaaabbab",
"abababbaabaababaabbaababbbaaaababaaabbababbababa",
"babaabbbbabbaaaabbaaaaab",
"aaaaaabbbbbbbaabaaaaabaaabbbbbbaaabbbbabbaabaaabbabaabab",
"abaabbaabbbbbbbbaaaaaaababaabbbabbabaaaa",
"baabbbbaaaabbababaabaaaa",
"aabaababaaaabbaaaababaaa",
"aabbabaabbbabaabbaabbbaababaabaa",
"aaaaaabaabbbaabaabaaabba",
"aaaaaabaabaaaaabababbabbbabaabbaaabbbbaaaaabbaaaabbaabaa",
"abbbaabbbaabbbbaabbbaaaabbbaaababbbbaabaabbababbbbbbaabbaaaabaab",
"baabbbbbabaabbbbabababaabbbaaaba",
"aaaabbaaabbabbabbabaabbb",
"bbaaabbabaaabbbaaabbaaabbaaaabaa",
"babbaaaaabaabbbaabaabbbaabbbbaaa",
"bbaaabbabbbaababbbabbabbbaabbaababaaabbaaabbabba",
"abaabbaabbabaabbbabbbaaaabaaaabb",
"abbabababbbabbabbabbabbbbbaaaaaaaaaabbbbabbabbaaaabbabaa",
"bbaabaabababbbbbaabababa",
"aabbbaabbabbbaabaabaaaaa",
"bbbbbbabaabaababbaaababb",
"bbbbbaabaaabbaababbbbabbaabbbbbbaabaaabbabaababaaabbababbbbbababbbbbabbabaababbaabbabaab",
"abbaaaabbbabbbbabbbbaabaabbbabaa",
"ababaaaabbabaabbabbbbaabbbbbbbabbaaabaaa",
"abbaaaaaaababbbabbaabaaa",
"bababaaaabbbbababaabbbabbbbbabab",
"aabbbbabaababbaaaabaaaaa",
"babbbaabbbabbaabaababbabaaaaabab",
"aaaaaababbaabaabbbababbaaaaabbbb",
"bababaabbaaaaaaaabbbabba",
"aaabaaabbabababbbbbbabbaaaabaaababbbaabbaaaaababbaaabbbbbbaabbbbabbabababaabaabb",
"bbabbbbaaaaaabaaababaaba",
"aaaabbbaaaaababbaabaaababababababbbbaababaaaaaaaababaababbbbababaaaabaaaaaabaabb",
"abbababaaababaabababbabb",
"aababbaabbbbabbaababbabb",
"bbbbabaabababaaaaabbbabb",
"babaababbbbbaabaababaabbbbbabaab",
"aabbbbaaaabbababaaaabbaabbbbaabaababbaaaabbaabab",
"baabbbababaabbaaabbbbaabaabaaabbaabbbabb",
"bbaaabbaabbabaabbbababaa",
"baaaababababbbbbbbaaababababbaaabbabbaaaaaaabbbbbabbbaaaababbaab",
"babbabbbaababbbbbaababbb",
"bababaaabbaabaabbaabaabbaaababbabbbbaabbaaababab",
"bbabaaabbababbbbbbbabbabaababbbaaaababaa",
"aaabbababbaaaababbaabbbb",
"bbbbaaababaabbbabbbabbbb",
"bbababbaabbaaaababaaaaaa",
"bbbaaabaaaaabaabbabababb",
"aaabaaaabbbbaabbbaaaaaaa",
"bbabbaabaaabbbbaaaababba",
"bbbaaaaaabbbaabaaababbaaaaabaaab",
"abaaabbbababaaabbbbaaababaaabaabaabaabbabbbabbabbbaaaaaaaabbabaa",
"aabbabbaababbabbbbbbbbba",
"baabbbbaaabababbabaabaab",
"aaaababaaabbbbbaaaaaabbb",
"bbbbbaaabababbbbababababaaabaaab",
"ababbbbbbbababbabbabbbbaabbababa",
"babaaabaaaabbabaaabbaabb",
"abaabababbbabbabbbbbabaababaabab",
"abbbababbabababababaabba",
"bbaabbaabaabbbababababbabababbbabaabaabbaabbabab",
"bbaababaaabbbaababbbbbbaaaaabbba",
"baaabbabbbbabbabbbaabaaa",
"abbabbbbbbbaabababaaabaa",
"baaaabbbabaabbaaabbaabaa",
"abbbabbbbbbbabbbaabaaabbbbbbaabbbaabbabbbaaabbab",
"bbbabaababbaababbbabaaba",
"ababbbbabaaabbbaaaabaaaa",
"ababbaaabbabaaaabaababba",
"abbaaaabbbaabaabbabbabab",
"aaabbbbbabbbaabbbaaabbbbabaaaaab",
"bababbbbbaaabbabbbaaabab",
"babbbaabbbabbbbaabbbaabb",
"bbaababbbbabbabaaabaababaababaaa",
"abaababaabbabbaaabbbbaaa",
"bbababbabbbbbaaabbababaa",
"aaaaaabaaabbbaaaabaaaabaaaabaaba",
"bbababbabaaabbbababbabaa",
"bbbabbabbbbababbabbbaaaaaabbbaaaabbbbaaabbbaababbbbaaaaababbaaababbaabaa",
"bbbbbaabaaaaabbaaaaabbaabbaaabbaabbabaabbaaaaabb",
"bbbbabbabbabaaabababaaba",
"aaabaabbbbaabaababbabbaabbbbbaabaababbbaabbaabbabbabbbaaabbbabaababbbbbb",
"abaabbbabbbbaabaabbaaaaaaabaaaab",
"abbbbbbbabaabababaaabbbabaaabbbabaaababbbabaaabbabbbabaa",
"abbabaaabaaabbbaaababaaa",
"baabbbbaaaababbaaabaababababbbabbabbaaba",
"bbaabababbaabbaaabbbabbbaaabbaaa",
"abbaababbabbbbbbabbaabaa",
"bbaababaaaabbabaaababbabbbaababbbababaabbbbbbabb",
"bbbbbaabaaaabaaabaaaabab",
"abbabbaaabbbaaaabbbbbaba",
"bbbbaabaaabbaaabbbbbbaabaabbabaaaababaaa",
"abbabbabaabbabbbabbbbabaabbbabbabababbab",
"aabbaaabbbabaabbaaaabbbb",
"bbabbaaababaaababbbaababbbaabbbbaabbaabbbbbbbaba",
"abababababbaaaaaabbaaaabbbbbbabbaabaaaba",
"bbbaabbababaaabaaaabbbbababbbbabbbbbabababaaabbb",
"bbaababbabbaaaaaabbbababbababbab",
"bbabbaaaaaaabbaaababaaaa",
"aababbabbbaaabaabbbbabbb",
"aaababbabaabbbbabbabaabbabbbababbbbbaaaaaabbbbbabaaaaaaa",
"aabbaaabaaababbbbababbbbabaabaaa",
"aaaaabababbbbbaabbbaaababbbaaaaaabbaaaab",
"aaaaaabbbbbabaababaababb",
"ababbbbbaaaaaababbbbbaba",
"baabbabaababbaabbbabbbbaaaaabaabaabbabbabaaaabab",
"abbbababbbbbaaaaabbbbbabbbabaaababbaaabaababbabbaaaabbaabbaabaaabaabbbbb",
"abbbbabaaabbbbabaabbbaaa",
"bbabaaababbabbaabababaaababaabaa",
"abababababbabbbbaaabbbaa",
"aababbabbabbbababaaaababbbbbbbaaabbaabbb",
"abababbaabbbbabbbabaabbbabbbaaaabbabbaaaabaaaaab",
"bbbbabaaababbabaaaabbabaabbabaaaabaababb",
"aabbaaabbababaaaaabaaaab",
"abbbababaabbbbbabbabbababbabbaabbbaaaabbbbbbabaababaabab",
"aabbabbababababbabbaabbbabbbabbb",
"babababbabaaaabaabbbaaabbbaabbbb",
"babaaabababaaaabaabbabbbababababbbababbabbbaaabbaabbabab",
"babaaaabbbabbbbaabaabaab",
"baaabbbabaabbbbabaaabaaa",
"ababbbbbabaabababaabbaabbbbbbbabbababaab",
"aaabaabbaaababbbbabbabab",
"aaaaaabaaaaabaabbaabbababbbabbaabaaaaabbbaabbbbbbaaaabab",
"aabaaabbbaaaabaabaaabbbbbabbaaabaaaabababbbaabbbbababaababaaabbbabaaaababaabbabb",
"aaaabaabaabbabaababbbaaa",
"babaabbbaabaabbaaabbaabbaabaabaa",
"bbabbbbbababababaabbbbbabaaaaabbabbababa",
"aabbbaaaaaaabbaaaabababb",
"babbbaaabbabbbbaaaaabaaabbaabbbb",
"aabaabbbbbaaabaabababbbaabaaaaab",
"bbbaababbbababbabbaababaaababbaabaaaabab",
"bbaabaabbaabaabaabbaaabb",
"aababbaabbaaabbbaaabbabaaabaabbbaaababbabababbabbbaaaabb",
"abaabbaabbabbaabbbbabbabbaababaaababbabb",
"baabbbaabbbbbaabbabbabaa",
"aaaaaaaaababbaabaababbbaababababaaaaaaabaabbbbaa",
"bbabaaabbabababaaaabbabababbbbaaabbbbaaa",
"baabbaabaabbaaabbbababaa",
"baababbbaaabbaabbbaaabab",
"abaaababaaabababaabaaaab",
"aaabbbbaabbbbabaaaabbababbbababbabbabaababbbbbbaaabbbabbbaababbbbaaabbbb",
"baaababbaababbbabbbbaaaabbbababbbbbabbaa",
"abbbbaaabaaabbabaababbbabbbbbbbabaabbbbbabbaaabbabbbbaaa",
"bbabaaabaaaabababbabbaaaaaaaaaabaaabbbbb"};

#pragma endregion Input

vector<string> str_split(const string str, const string tok)
{
    assert(tok.size() > 0);
    vector<string> retVal = {};
    string tmp = str;
    int tokSize = tok.size();
    size_t findPos = tmp.find(tok);
    while(findPos < string::npos)
    {
        retVal.push_back(tmp.substr(0,findPos));
        if (findPos + tokSize < tmp.size())
        {
            tmp = tmp.substr(findPos + tokSize);
        }
        else
        {
            tmp = "";
            break;
        }
        findPos = tmp.find(tok);
    }

    retVal.push_back(tmp);

    return retVal;
}

vector<string> vstr_filter(const vector<string> in, bool(*pred)(const string))
{
    vector<string> retVal = {};

    for (const string s : in)
    {
        if (pred(s))
        {
            retVal.push_back(s);
        }
    }

    return retVal;
}

bool isEmptyStr(const string s)
{
    return s.size() == 0;
}

bool isNotEmptyStr(const string s)
{
    return !isEmptyStr(s);
}

typedef struct Rule
{
    typedef enum { Term, RuleNum } Type;
    Type type;
    char terminal='!';
    int ruleNum = -1;
} Rule;

struct GRMResult;

typedef struct GRMNode
{
    int id = -1;
    vector<vector<Rule>> rawSubRuleSets;
} GRMNode;

typedef struct GRMResult
{
    string left = "x";
    bool success = false;
} GRMResult;

vector<GRMResult> vGRMResult_filter(const vector<GRMResult> in, bool(*pred)(const GRMResult))
{
    vector<GRMResult> retVal = {};

    for (const GRMResult s : in)
    {
        if (pred(s))
        {
            retVal.push_back(s);
        }
    }

    return retVal;
}


GRMResult MatchTerminal(const string str, const Rule rule)
{
    assert(rule.type == Rule::Type::Term);

    if (str.size() == 0)
    {
        return {};
    }

    string cpy = str;
    bool passedRule = rule.terminal == str[0];
    
    if (cpy.size() > 1)
    {
        cpy = cpy.substr(1);
    }
    else
    {
        cpy = "";
    }

    return {cpy, passedRule};
}

vector<GRMResult> MatchNode(const string str, const GRMNode node, const map<int, GRMNode> nodes);

vector<GRMResult> MatchRuleRef(const string str, const Rule rule, const map<int,GRMNode> nodes)
{
    assert(rule.type == Rule::Type::RuleNum);
    int nextRuleId = rule.ruleNum;
    return MatchNode(str,nodes.find(nextRuleId)->second,nodes);
}

vector<GRMResult> MatchSeqRules(const vector<GRMResult> strs, const vector<Rule> rules, const map<int,GRMNode> nodes)
{
    vector<GRMResult> conts = strs;

    for (const Rule rule : rules)
    {
        vector<GRMResult> toAdd = {};
        for (GRMResult& cont : conts)
        {
            if (!cont.success)
            {
                continue;
            }

            switch (rule.type)
            {
                case Rule::Type::Term:
                {
                    GRMResult rres = MatchTerminal(cont.left,rule);
                    if (rres.success == true)
                    {
                        cont.left = rres.left;
                    }
                    else
                    {
                        cont.success = false;
                        cont.left = "";
                    }
                    break;
                }

                case Rule::Type::RuleNum:
                {
                    vector<GRMResult> reses = MatchRuleRef(cont.left,rule,nodes);
                    if (reses.size() > 0)
                    {
                        toAdd.insert(toAdd.end(), reses.begin(), reses.end());
                    }
                    // mark this cont as not success full regardless
                    // if it succeeded we have other ones to check
                    // it it didn't oh well
                    cont.success = false;
                    cont.left = "";
                    break;
                }

                default:
                    assert(false);
                    break;
            }
        }
        conts.insert(conts.end(),toAdd.begin(),toAdd.end());
        conts = vGRMResult_filter(conts, [](GRMResult r){return r.success;});
    }
    
    return conts;
}

vector<GRMResult> MatchNode(const string str, const GRMNode node, const map<int, GRMNode> nodes)
{
    vector<GRMResult> ret = {};
    
    if(str.size() > 0)
    {
        GRMResult res;
        vector<GRMResult> ruleRet;
        for (const auto orRule : node.rawSubRuleSets)
        {
            // SLOW AS FUCK, do we really need to process all branches?
            // consider only evaluating branches if needed
            res = {str, true};
            ruleRet = MatchSeqRules({res},orRule,nodes);
            ret.insert(ret.end(),ruleRet.begin(),ruleRet.end());
        }
    }

    return ret;
}

map<int, GRMNode>makeRawRules(vector<string> rules)
{
    map<int, GRMNode> nodes;
    for (const string rule : rules)
    {
        GRMNode node = {};
        node.id = atoi(rule.substr(0,rule.find(':')).c_str());
        string subRules = rule.substr(rule.find(':')+2);
        
        vector<string> orRules = vstr_filter(str_split(subRules,"|"), isNotEmptyStr);

        for (size_t i = 0; i < orRules.size(); i++)
        {
            node.rawSubRuleSets.push_back({});
            vector<string> subRules = vstr_filter(str_split(orRules[i]," "), isNotEmptyStr);
            for (size_t j = 0; j < subRules.size(); j++)
            {
                Rule r = {};
                if (subRules.size() == 1 && isalpha(subRules[j][0]))
                {
                    r.type = Rule::Type::Term;
                    r.terminal = subRules[j][0];
                }
                else
                {
                    r.type = Rule::Type::RuleNum;
                    r.ruleNum = atoi(subRules[j].c_str());
                }
                node.rawSubRuleSets[i].push_back(r);
            }   
        }

        nodes.insert({node.id, node});
    }
    
    return nodes;
}

int main()
{
    cout<<"Procesing input (internally stored)..."<<endl;
    map<int,GRMNode> rules = makeRawRules(inputRules);
    GRMNode node0 = rules[0];

    int sumt = 0;
    cout<<"Sol1 is slow, please have patience."<<endl;
    for (const auto line : inputLines)
    {
        auto a = vGRMResult_filter(MatchNode(line,node0,rules), [](GRMResult r){return r.left == "" && r.success;});
        if (a.size() > 0)
        {
            ++sumt;
        }

    }
    cout<<endl<<
    "---------------------------------"<<endl<<
    "        s1: "<<sumt<<endl<<
    "---------------------------------"<<endl<<endl;

    rules[8].rawSubRuleSets.push_back({{Rule::Type::RuleNum,'!',42},{Rule::Type::RuleNum,'!',8}});
    rules[11].rawSubRuleSets.push_back({{Rule::Type::RuleNum,'!',42},{Rule::Type::RuleNum,'!',11},{Rule::Type::RuleNum,'!',31}});

    sumt = 0;
    cout<<"Sol2 is slow as fuck boi."<<endl
    <<"Better make some tea, go read a book, travel the world, do something productive with your life."<<endl
    <<"This will still be processing when you're done."<<endl;
    for (const auto line : inputLines)
    {
        auto b = MatchNode(line,node0,rules);
        auto a = vGRMResult_filter(b, [](GRMResult r){return r.left == "" && r.success;});
        if (a.size() > 0)
        {
            ++sumt;
        }
    }
    cout<<endl<<
    "---------------------------------"<<endl<<
    "        s1: "<<sumt<<endl<<
    "---------------------------------"<<endl;

    return 0;
}