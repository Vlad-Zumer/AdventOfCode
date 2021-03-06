import qualified Data.List
import qualified Data.Char
import qualified Data.Map as Map

data Bit = Zero | One deriving(Show)
data Trit = Trit Bit | X deriving(Show)

type Trits = [Trit]
type Bits = [Bit]

newtype Mask = Mask Trits deriving(Show)
newtype Mem = Mem (Int, Int) deriving(Show)

data Token =  TMask Mask | TMem Mem deriving(Show)

newtype ProgramState = PState (Mask, Map.Map Int Int) deriving(Show)

stringToTrits :: String -> Trits
stringToTrits [] = []
stringToTrits ('0':ls) = Trit Zero:(stringToTrits ls)
stringToTrits ('1':ls) = Trit One:(stringToTrits ls)
stringToTrits ('X':ls) = X:(stringToTrits ls)
stringToTrits ('x':ls) = X:(stringToTrits ls)
stringToTrits s = error $ "String could not be parsed to trit: " ++ s

intToBits :: Int -> Bits
intToBits 0 = [Zero]
intToBits 1 = [One]
intToBits n
    | even n = intToBits (n `div` 2) ++ [Zero]
    | otherwise  = intToBits (n `div` 2) ++ [One]

bitsToInt :: Bits -> Int
bitsToInt bs = bitsToIntAcc bs 0
    where
        bitsToIntAcc :: Bits -> Int -> Int
        bitsToIntAcc [] acc = acc
        bitsToIntAcc (One:bss) acc = bitsToIntAcc bss (acc * 2 + 1)
        bitsToIntAcc (Zero:bss) acc = bitsToIntAcc bss (acc * 2)

bitsToTrits :: Bits -> Trits
bitsToTrits = map Trit

applyMask :: Trits -> Bits -> Bits
applyMask [] [] = []
applyMask ((Trit Zero):trits) (_:bits) = Zero:applyMask trits bits
applyMask ((Trit One):trits) (_:bits) = One:applyMask trits bits
applyMask (X:trits) (b:bits) = b:applyMask trits bits
applyMask _ _ = error "Arrays of different len!"

padWith0To36Bit :: Bits -> Bits
padWith0To36Bit bits
    | length bits == 36 = bits
    | length bits < 36 = [Zero | i <- [1 .. 36 - length bits]] ++ bits
    | otherwise  = error "Already too many bits"

padWith1To36Bit :: Bits -> Bits
padWith1To36Bit bits
    | length bits == 36 = bits
    | length bits < 36 = [One | i <- [1 .. 36 - length bits]] ++ bits
    | otherwise  = error "Already too many bits"

padWithXTo36Trit :: Trits -> Trits
padWithXTo36Trit trits
    | length trits == 36 = trits
    | length trits < 36 = [X | i <- [1 .. 36 - length trits]] ++ trits
    | otherwise  = error "Already too many bits"


parseLine :: String -> Token
parseLine line
    | "mask" `Data.List.isPrefixOf` line = TMask . parseMask $ line
    | "mem" `Data.List.isPrefixOf` line = TMem . parseMem $ line
    | otherwise  = error ("Unrecognized command: " ++ line)
        where
            parseMask :: String -> Mask
            parseMask line = case Data.List.stripPrefix "mask = " line of
              Nothing -> error $ "Cannot parse mask in line: " ++ line
              Just s -> Mask $ stringToTrits s


            parseMem :: String -> Mem
            parseMem line = Mem (read loc :: Int, read val :: Int)
                where
                    loc :: String
                    loc = takeWhile Data.Char.isDigit . dropWhile (not . Data.Char.isDigit) $ line
                    val :: String
                    val = takeWhile Data.Char.isDigit . dropWhile (not . Data.Char.isDigit) . dropWhile (/= '=') $ line

evalProgram1 :: ProgramState -> Token -> ProgramState
evalProgram1 (PState (mask, memory)) (TMask newMask) = PState (newMask, memory)
evalProgram1 (PState (Mask mask, memory)) (TMem newMem) = PState (Mask mask, updateMemory memory newMem)
    where
        updateMemory :: Map.Map Int Int -> Mem -> Map.Map Int Int
        updateMemory map (Mem new)
            | Map.null map = uncurry Map.singleton new
            | otherwise = uncurry Map.insert new map
        applyLocalMask :: Mem -> Mem
        applyLocalMask (Mem (id, val)) = Mem (id, bitsToInt . applyMask (padWithXTo36Trit mask) $ (padWith0To36Bit . intToBits $ val))

evalProgram2 :: ProgramState -> Token -> ProgramState
evalProgram2 (PState (mask, memory)) (TMask newMask) = PState (newMask, memory)
evalProgram2 (PState (Mask mask, memory)) (TMem newMem) = PState (Mask mask, updateMemory memory newMem)
    where
        updateMemory :: Map.Map Int Int -> Mem -> Map.Map Int Int
        updateMemory mem new = foldl (flip (uncurry Map.insert)) mem (map unpackMemory (applyLocalMask new))

        unpackMemory :: Mem -> (Int, Int)
        unpackMemory (Mem (id, val)) = (id, val)

        applyLocalMask :: Mem -> [Mem]
        applyLocalMask (Mem (id, val)) = [Mem (address, val)| address <- (map bitsToInt (applyMaskToAddress mask (padWith0To36Bit . intToBits $ id)))]

        applyMaskToAddress :: Trits -> Bits -> [Bits]
        applyMaskToAddress [] [] = [[]]
        applyMaskToAddress ((Trit Zero):trits) (b:bits) = map ([b] ++) (applyMaskToAddress trits bits)
        applyMaskToAddress ((Trit One):trits) (b:bits) = map ([One] ++) (applyMaskToAddress trits bits)
        applyMaskToAddress (X:trits) (b:bits) = map ([One]++) (applyMaskToAddress trits bits) ++ map ([Zero]++) (applyMaskToAddress trits bits)
        applyMaskToAddress _ _ = error "Different len arrays!"

sol1 :: [String] -> Int
sol1 input = sumMem doneState
    where
        doneState :: ProgramState
        doneState = foldl evalProgram1 initProgram (map parseLine input)
        initProgram :: ProgramState
        initProgram = PState (Mask . padWithXTo36Trit $ [], Map.empty )
        sumMem :: ProgramState -> Int
        sumMem (PState (_ , map)) = sum map

sol2 :: [String] -> Int
sol2 input = sumMem doneState
    where
        doneState :: ProgramState
        doneState = foldl evalProgram2 initProgram (map parseLine input)
        initProgram :: ProgramState
        initProgram = PState (Mask . padWithXTo36Trit $ [], Map.empty )
        sumMem :: ProgramState -> Int
        sumMem (PState (_ , map)) = sum map

main :: IO()
main = do
    print $ sol1 inputLines
    print $ sol2 inputLines

inputLines :: [String]
inputLines = ["mask = 010X1100101X00X01001X11010X111100X01",
    "mem[23014] = 9778",
    "mem[42882] = 140716",
    "mem[65461] = 458355100",
    "mem[60151] = 31172",
    "mem[47143] = 7055",
    "mask = X101XX001XX1001010X101X1101011100101",
    "mem[26134] = 4394",
    "mem[18808] = 352500",
    "mem[18556] = 87307674",
    "mask = X11X110X1011X11X0001011000000X110000",
    "mem[49018] = 14063312",
    "mem[42211] = 45834020",
    "mem[36848] = 5772919",
    "mask = X1X1110110100001100X000100111X001X01",
    "mem[9118] = 531662307",
    "mem[31896] = 259680444",
    "mem[35532] = 125291736",
    "mem[1284] = 696970",
    "mask = 01X01100001000001X01000X100X101X0000",
    "mem[54966] = 1480453",
    "mem[37281] = 5032",
    "mask = 1X0101XXXX1X1X10X0X00101010100101001",
    "mem[9696] = 1216",
    "mem[15633] = 2716652",
    "mem[42329] = 570596065",
    "mem[45651] = 592043",
    "mem[53599] = 84957",
    "mask = 1X0X11101011X0101000X010011101010000",
    "mem[782] = 131830570",
    "mem[17999] = 26010047",
    "mem[10590] = 55862652",
    "mem[41166] = 1571",
    "mem[52245] = 1080732",
    "mem[60973] = 10973",
    "mask = X1X1110X10110X10X001111XXXX011110011",
    "mem[31425] = 3079",
    "mem[3872] = 12709",
    "mem[44738] = 1520775",
    "mem[6214] = 218610777",
    "mask = 1001X111X1X00X1010XX1X11X1110X000010",
    "mem[915] = 2006",
    "mem[20530] = 208698283",
    "mem[59288] = 176283",
    "mask = 100111X111XXXX101X0011X100100100X000",
    "mem[65348] = 99675",
    "mem[27552] = 1331058",
    "mem[40973] = 96189",
    "mem[10170] = 4651459",
    "mem[63354] = 32215",
    "mem[32591] = 233262264",
    "mask = 01000XX11011X00XX0101X11X011X0111101",
    "mem[1947] = 6511663",
    "mem[60231] = 42099957",
    "mask = X101X10111XX111X1010011100010011100X",
    "mem[62520] = 63838349",
    "mem[39257] = 438174908",
    "mem[44390] = 54942",
    "mem[9596] = 13923813",
    "mem[64877] = 1031",
    "mem[54384] = 198269",
    "mask = 1X0111001011XX10101000100110X000X000",
    "mem[6214] = 4758",
    "mem[59288] = 7427700",
    "mem[22338] = 51826811",
    "mem[17999] = 3215",
    "mem[21407] = 136757",
    "mask = 11X110001X1100101X0X0111XX0X0XX10101",
    "mem[19064] = 546660",
    "mem[21384] = 2362194",
    "mem[23581] = 198371",
    "mem[15213] = 6998212",
    "mem[33314] = 22741",
    "mem[1308] = 198573",
    "mask = 1111110110110110XXX110100100X101X1XX",
    "mem[5021] = 18730",
    "mem[45881] = 3729",
    "mem[41145] = 560790",
    "mem[17059] = 335092590",
    "mem[49349] = 506836719",
    "mask = 1X011100101X00X01X0001101000X1XXX101",
    "mem[47308] = 232133",
    "mem[3939] = 3046",
    "mem[57467] = 181551031",
    "mem[34672] = 6233509",
    "mem[15155] = 866",
    "mask = 110111001X1X1010X0010010000X0011X111",
    "mem[6377] = 3902382",
    "mem[11542] = 1194",
    "mem[52887] = 30306498",
    "mem[49605] = 437472",
    "mem[34100] = 27149",
    "mask = 1101110101X1XX01101010001X1110000000",
    "mem[52729] = 41283384",
    "mem[30818] = 2619040",
    "mem[15175] = 51188968",
    "mem[40793] = 19775",
    "mem[24243] = 3676134",
    "mask = 110110X0111X0010100101X1X101000X0010",
    "mem[57775] = 1687",
    "mem[50711] = 15613",
    "mem[4712] = 67911",
    "mem[24243] = 827",
    "mask = 1101X10X10111X101000X00X11000X010001",
    "mem[32109] = 12583776",
    "mem[39041] = 76972021",
    "mem[57467] = 9561",
    "mem[12331] = 62157219",
    "mem[35060] = 4081357",
    "mem[42738] = 53814734",
    "mask = 1111X10XX011X010101101011001010011X0",
    "mem[42637] = 3384964",
    "mem[27323] = 5296894",
    "mem[30389] = 36",
    "mask = 1X1011X010X000X01X110101XX1001011X1X",
    "mem[37471] = 1911918",
    "mem[34853] = 606298375",
    "mem[34692] = 8502805",
    "mem[51588] = 499294838",
    "mem[46504] = 96337",
    "mask = 10011X01110XXX10110X1111110X0000XX00",
    "mem[12036] = 4251895",
    "mem[47948] = 14594581",
    "mem[57780] = 50914073",
    "mem[27753] = 73410286",
    "mask = 01000100101X001010X1X0000001100X1000",
    "mem[29981] = 25900",
    "mem[2063] = 1329",
    "mem[2215] = 81989",
    "mask = 10XX1X111X10011011011X111X1X11011X11",
    "mem[45276] = 74535",
    "mem[47375] = 1924385",
    "mem[18011] = 144102",
    "mem[2215] = 446755",
    "mem[21848] = 361000897",
    "mem[33096] = 348003231",
    "mem[2261] = 49696039",
    "mask = 010X1101101100011X1001011X101X000100",
    "mem[21742] = 64308",
    "mem[38665] = 125990",
    "mem[47858] = 27335545",
    "mem[62873] = 54978664",
    "mem[18436] = 710957327",
    "mem[32157] = 39549",
    "mask = X010X10XX0000000101111000001XX00XX11",
    "mem[53917] = 8231128",
    "mem[10404] = 377",
    "mem[24243] = 19",
    "mask = X110110010100XX0111101010111X0000010",
    "mem[43071] = 722653",
    "mem[55599] = 4022954",
    "mem[19270] = 5382",
    "mem[4864] = 2901",
    "mem[12055] = 1008",
    "mask = X10010101110XX0011100100X101110011X0",
    "mem[55666] = 3461",
    "mem[1348] = 3490",
    "mem[26481] = 122709",
    "mem[35986] = 882775",
    "mem[14766] = 3144",
    "mask = 1100110010X00X1X10X00101100XX0000101",
    "mem[2475] = 99253",
    "mem[34853] = 104692938",
    "mem[45799] = 207244716",
    "mask = 11011101101000100000X1X111101X1111X1",
    "mem[41278] = 6462",
    "mem[63229] = 2337",
    "mem[30241] = 417551945",
    "mem[43695] = 32632984",
    "mem[55872] = 4732314",
    "mask = 110X1X0110110XX0101XXX11000001101001",
    "mem[30748] = 437150551",
    "mem[23881] = 13008",
    "mem[5931] = 203469684",
    "mask = X1XX1100101000X0X0XX01X0100000000101",
    "mem[39898] = 2937",
    "mem[47140] = 33506",
    "mem[47586] = 426",
    "mem[15233] = 746123",
    "mask = 1X0X1XX011100X00111001XX0X0010X01100",
    "mem[21781] = 933",
    "mem[448] = 31309385",
    "mem[2370] = 147491885",
    "mem[17999] = 9983",
    "mem[53599] = 114654",
    "mem[31772] = 21352913",
    "mask = 010X1100101000XX10X1X010100X1100010X",
    "mem[63720] = 344628057",
    "mem[22627] = 1194485",
    "mask = 1X111100X0110110100X1000X11001XX0X1X",
    "mem[24788] = 12640",
    "mem[46948] = 32700",
    "mem[37444] = 18654",
    "mem[17828] = 529955",
    "mem[62908] = 9264688",
    "mask = X1X011X1101100X1XX1X011X01010010X001",
    "mem[38068] = 838",
    "mem[10337] = 95750",
    "mask = X10X110110011XXX1001011100X010000110",
    "mem[44104] = 2125128",
    "mem[5124] = 2138255",
    "mem[48215] = 8302410",
    "mem[55198] = 1768",
    "mem[1004] = 78304",
    "mask = X0X111X01010X00X1100X11001000100X101",
    "mem[831] = 60849768",
    "mem[10543] = 16435",
    "mem[16484] = 2867",
    "mem[50932] = 12631",
    "mem[18011] = 604087933",
    "mem[41312] = 6603336",
    "mask = 1X1X11X010X0001011111101001XX1011100",
    "mem[3983] = 424012576",
    "mem[44104] = 2231",
    "mem[48062] = 166670",
    "mem[60570] = 51507045",
    "mem[16895] = 4480",
    "mask = 0X0011X00010000010X01X01100100000001",
    "mem[43729] = 305741",
    "mem[58297] = 478012",
    "mem[37903] = 247336",
    "mem[49003] = 264571",
    "mem[21768] = 5198725",
    "mem[20421] = 62589",
    "mask = 1111110010110X1X10X1X110100X0101X0X1",
    "mem[36083] = 15401",
    "mem[30389] = 1361",
    "mem[23220] = 28363",
    "mem[20840] = 123771110",
    "mem[49892] = 40904",
    "mem[32109] = 2027",
    "mem[27148] = 446669",
    "mask = 1010X10100X00000101X1110000X10101011",
    "mem[10592] = 21795764",
    "mem[60581] = 37413",
    "mem[24621] = 3373323",
    "mem[37811] = 1221059",
    "mem[38216] = 8082306",
    "mem[41142] = 4906074",
    "mask = X10XXX11011110011X00X0X1111X1010X100",
    "mem[11928] = 502744425",
    "mem[27552] = 3101566",
    "mem[1674] = 5000288",
    "mask = 1X0X100X111001X0X11011X00000011X011X",
    "mem[33228] = 20563",
    "mem[59224] = 2021",
    "mem[58230] = 2931",
    "mask = 1X001000XX100100110001000X0010X0010X",
    "mem[12037] = 14972561",
    "mem[63354] = 134775",
    "mem[2111] = 462039547",
    "mem[16412] = 16419724",
    "mem[61945] = 582",
    "mem[53181] = 265634",
    "mask = 1100110110110X1X1010X1010101000010XX",
    "mem[27753] = 3434",
    "mem[24997] = 7421633",
    "mem[54772] = 2074596",
    "mem[53599] = 950852",
    "mem[58552] = 1454",
    "mask = 11X111001011000XX000001011X1101X1101",
    "mem[17491] = 8295753",
    "mem[44735] = 74648",
    "mem[58213] = 2842642",
    "mem[11964] = 15593270",
    "mem[20822] = 341",
    "mask = 1XX101011X10X01010XX0111100X10X10110",
    "mem[7947] = 6206",
    "mem[22164] = 5853463",
    "mem[23138] = 6813",
    "mem[16076] = 16062",
    "mem[10225] = 8667538",
    "mem[37506] = 421615",
    "mask = 1X01X101101000101X00X1011X000011010X",
    "mem[28704] = 154155",
    "mem[47830] = 1243",
    "mem[1631] = 1060",
    "mem[4251] = 73313",
    "mem[26709] = 71",
    "mem[39041] = 3374305",
    "mask = 1101110010110010100X1110X01X1001010X",
    "mem[29270] = 58295",
    "mem[31307] = 106482665",
    "mem[47830] = 2877",
    "mem[1949] = 9008841",
    "mask = 111011X0101X0X001111X101X11X0X00X010",
    "mem[19816] = 5499",
    "mem[48215] = 996",
    "mem[8255] = 450711",
    "mem[56960] = 661",
    "mem[33808] = 2564351",
    "mem[3775] = 4613153",
    "mem[41658] = 75277805",
    "mask = XX0X1100101X001010001011100XXX100100",
    "mem[53504] = 3114",
    "mem[53599] = 143",
    "mem[49459] = 93723075",
    "mem[63493] = 1922",
    "mem[10635] = 5387",
    "mask = 000XX0100X1011000010X0X00X0010001X10",
    "mem[29981] = 38520",
    "mem[20980] = 6607014",
    "mem[21516] = 1785",
    "mask = 11011100XX100010X100111100X00100X10X",
    "mem[20992] = 10364",
    "mem[1182] = 2181",
    "mask = 10011000X11X0100111X110X010110110111",
    "mem[58228] = 816",
    "mem[52946] = 1046216022",
    "mem[38233] = 66872",
    "mem[53190] = 1070814007",
    "mem[33572] = 265",
    "mem[53495] = 48514858",
    "mem[33803] = 101117778",
    "mask = 100111X01X1001X0111000X0110010000000",
    "mem[25934] = 314853",
    "mem[20986] = 2338",
    "mem[49915] = 30330",
    "mem[44266] = 344908",
    "mask = 0100X1X110110X01101001101X0000000X01",
    "mem[49827] = 16630",
    "mem[131] = 7478808",
    "mem[49018] = 236053399",
    "mem[41329] = 523544",
    "mem[32104] = 12492",
    "mem[65106] = 28",
    "mem[5021] = 23010383",
    "mask = 010111XX10X1X010100101X001XX10010110",
    "mem[3738] = 681",
    "mem[12113] = 276035",
    "mem[45070] = 22799792",
    "mem[48859] = 12803",
    "mem[51303] = 3011402",
    "mem[19889] = 52441",
    "mask = 110X1XXX1110X110X11011X0000101X1011X",
    "mem[17200] = 104385",
    "mem[39448] = 14051",
    "mem[54966] = 3057321",
    "mem[55265] = 126636452",
    "mem[51014] = 2496",
    "mem[18748] = 114442",
    "mem[64434] = 114146787",
    "mask = 1X0111XX101100X0X00X000110XX1X110100",
    "mem[23881] = 26471",
    "mem[37868] = 50050",
    "mem[21012] = 7626",
    "mask = 11111100101000101010X001X0000X10X0X1",
    "mem[25324] = 43020630",
    "mem[51716] = 887",
    "mem[46427] = 4427906",
    "mask = 10010110X11X10101010011X010110X000X1",
    "mem[35216] = 92656119",
    "mem[49148] = 1329635",
    "mem[55614] = 8697505",
    "mem[21609] = 7070",
    "mask = 010X110010110X1010011X10110011010X01",
    "mem[56499] = 3531",
    "mem[31750] = 12717539",
    "mem[45217] = 6190610",
    "mem[50900] = 1900",
    "mem[37889] = 3835804",
    "mask = 1X01X111X0110X1000001X1X00000X1001XX",
    "mem[37314] = 2350",
    "mem[39257] = 16400629",
    "mem[30823] = 207011",
    "mask = 1000100X111X010X1010011X001010X1011X",
    "mem[26688] = 5335",
    "mem[58884] = 720182",
    "mem[59369] = 879490",
    "mem[1054] = 1454804",
    "mask = 11X1X100101000X0X01X110010X000100101",
    "mem[20512] = 14779",
    "mem[18682] = 3020",
    "mem[49459] = 3973929",
    "mem[58367] = 42944838",
    "mask = 10XX110110100X1010100X10000001XXX1X0",
    "mem[53190] = 392",
    "mem[63621] = 117998",
    "mem[10102] = 1617031",
    "mem[14955] = 782264",
    "mem[63354] = 596",
    "mask = 1X01110X1X1X00X01XX0011000000000X10X",
    "mem[59288] = 32430162",
    "mem[37444] = 1991341",
    "mem[63601] = 23037",
    "mem[1125] = 28413106",
    "mem[55528] = 999",
    "mem[1739] = 6314",
    "mask = 1101110111100001101011X0011110X100XX",
    "mem[48859] = 4120",
    "mem[14563] = 21453090",
    "mem[1169] = 108115775",
    "mask = 1X0111XX101X0010X000111X100XX10001X1",
    "mem[32805] = 2441",
    "mem[65348] = 145914666",
    "mem[20281] = 6605",
    "mask = 1101X101XX1X000X10X0X000011110000X01",
    "mem[18808] = 215404803",
    "mem[29472] = 163575",
    "mem[55666] = 12037",
    "mem[38665] = 612758024",
    "mem[57876] = 422",
    "mem[37900] = 1807",
    "mem[55399] = 8193703",
    "mask = X0111100101X0X001100111001XX00XX110X",
    "mem[45070] = 7751",
    "mem[3435] = 936",
    "mem[40695] = 67093",
    "mem[5495] = 128880",
    "mem[46597] = 2850",
    "mask = 10011100X111001011X0011X011XX00X0100",
    "mem[21971] = 5249",
    "mem[3871] = 82795232",
    "mem[16943] = 8964191",
    "mem[50436] = 441",
    "mem[20421] = 195226",
    "mask = 00X0X01000101100XX100XX0X1X010X01110",
    "mem[34885] = 653677",
    "mem[8170] = 43079",
    "mem[5396] = 950",
    "mem[42073] = 252233",
    "mem[13486] = 831356",
    "mem[34199] = 20223110",
    "mask = 10011X1X11X100101100X1X011100100XX0X",
    "mem[40732] = 1022756493",
    "mem[14144] = 411357632",
    "mask = 1001X1X111101110X10011X0101111101000",
    "mem[57539] = 774",
    "mem[14536] = 38799961",
    "mask = X1X01XX0101XX1X0111111X100X110111000",
    "mem[9778] = 33256650",
    "mem[25551] = 1985",
    "mem[39257] = 51040068",
    "mem[56357] = 110816",
    "mem[42154] = 328899450",
    "mask = 100X100001XX1001X11010010011110XX1X0",
    "mem[1403] = 134849806",
    "mem[5964] = 62290391",
    "mem[4408] = 48393222",
    "mem[17577] = 11210413",
    "mem[24997] = 501527",
    "mem[46863] = 46372",
    "mask = X00010X00X10XX0X11100X0100111XX11110",
    "mem[23527] = 42133679",
    "mem[17884] = 226472",
    "mem[29432] = 4271361",
    "mem[33298] = 16179",
    "mask = X0011100111000X01X00101X0010X1XX0100",
    "mem[28406] = 5450921",
    "mem[20484] = 9212786",
    "mask = 1X0010X0X11001001XXX0100X0001X11110X",
    "mem[13174] = 1644",
    "mem[44927] = 6975289",
    "mem[16396] = 10219",
    "mem[12766] = 11541",
    "mem[51161] = 511339321",
    "mask = X111X10110X10010000111111000010100X1",
    "mem[41461] = 5770",
    "mem[21329] = 435055",
    "mem[18593] = 61190",
    "mem[53504] = 2376852",
    "mem[51949] = 816",
    "mem[35265] = 772",
    "mem[14928] = 4001",
    "mask = 1001110X10X00010000011X1111X0X000000",
    "mem[33539] = 79448",
    "mem[15175] = 11359",
    "mem[41658] = 25915189",
    "mem[2215] = 152778",
    "mask = 110111001011X010100XXX0010X00111XXXX",
    "mem[45802] = 3611",
    "mem[62901] = 458786565",
    "mem[29270] = 2017746",
    "mem[15866] = 42174056",
    "mem[37286] = 23843129",
    "mem[47143] = 4922",
    "mem[33954] = 1832",
    "mask = 110100001011X01X0X001000X000X0001010",
    "mem[2428] = 2134006",
    "mem[37027] = 1425",
    "mem[33228] = 5638391",
    "mask = 11001X00111001001110XX00001001X01X11",
    "mem[47000] = 462713",
    "mem[687] = 4022",
    "mem[58197] = 117943131",
    "mem[41166] = 283078",
    "mem[26245] = 24480281",
    "mem[8116] = 76873415",
    "mem[48859] = 848150489",
    "mask = X10011111011000110X001X10101011000XX",
    "mem[35187] = 310838803",
    "mem[43388] = 68828",
    "mem[36770] = 2843",
    "mem[30407] = 926152",
    "mem[30792] = 647",
    "mask = 1X01X100111X0000X110000X0110X110X111",
    "mem[22211] = 250064298",
    "mem[11873] = 452688990",
    "mem[33512] = 31039",
    "mask = 1X01111X1XX001101X01101101X0000000X0",
    "mem[24467] = 47938",
    "mem[16345] = 985",
    "mem[58797] = 74250371",
    "mem[31153] = 1012000",
    "mem[45881] = 290989873",
    "mem[41278] = 114999407",
    "mask = 110111001X11001010010X0010X110101X00",
    "mem[13326] = 406",
    "mem[4873] = 1226343",
    "mem[6514] = 5567445",
    "mem[60581] = 5224932",
    "mem[38493] = 2208262",
    "mem[7941] = 799",
    "mask = 1011X1001010XXX1110010X0011000X10101",
    "mem[55903] = 62282",
    "mem[17478] = 16429",
    "mask = 1111010111100X101000011X11X00X011010",
    "mem[10592] = 8939",
    "mem[1686] = 712739",
    "mem[63229] = 66200607",
    "mem[7997] = 40043182",
    "mem[16503] = 5584",
    "mem[11895] = 7827",
    "mem[13452] = 1788711",
    "mask = 11111X00X0110010100X011X1010X1010101",
    "mem[42976] = 3918",
    "mem[53917] = 118437",
    "mem[908] = 778888",
    "mem[35763] = 1355604",
    "mask = 1101XX001011X010X00X00X1000X00001X01",
    "mem[35247] = 1722533",
    "mem[44526] = 252097",
    "mem[32061] = 245369",
    "mem[12544] = 24885",
    "mem[6113] = 10982959",
    "mask = 1X011X0X1110001011001100010X00011100",
    "mem[4117] = 2058",
    "mem[42421] = 14118780",
    "mem[11833] = 11912377",
    "mem[22164] = 48",
    "mem[25501] = 3465263",
    "mask = 110X01X1011X10XX10000X10X0X1X0100000",
    "mem[44268] = 461719",
    "mem[30153] = 58345299",
    "mem[25475] = 233748959",
    "mem[25166] = 46886713",
    "mem[40732] = 346",
    "mask = 11X111X01011001X100X0101100011100000",
    "mem[46019] = 28244119",
    "mem[25240] = 4875676",
    "mask = 01000X10X01000101001X00X100110100101",
    "mem[11873] = 251836",
    "mem[35247] = 1896995",
    "mem[45217] = 30515968",
    "mem[5548] = 578580112",
    "mask = 0100X1X0X01000X0100X100010000XX00X01",
    "mem[16345] = 14491763",
    "mem[23881] = 138151",
    "mem[63524] = 67022",
    "mem[2475] = 222204469",
    "mem[37444] = 5882",
    "mem[6545] = 520"]