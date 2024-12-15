import Data.Map qualified as M

-- ghc -no-keep-hi-file -no-keep-o-files main.hs

parseInput :: String -> [Int]
parseInput content = map read (words content)

numOfChars :: Int -> Int
numOfChars x = length $ show x

splitNumber :: Int -> [Int]
splitNumber x = [read $ take size xstr, read $ drop size xstr]
  where
    size = (numOfChars x) `div` 2
    xstr = show x

applyRules :: Int -> [Int]
applyRules 0 = [1]
applyRules x
    | even (numOfChars x) = splitNumber x
    | otherwise = [x * 2024]

doIteration :: [Int] -> [Int]
doIteration input = input >>= applyRules

{-
 Part 2: Group by stone values and update in sync
-}
type Val = Int
type Count = Int
type StoneCount = M.Map Val Count

makeStoneCounts :: [Val] -> StoneCount
makeStoneCounts xs = foldl (\dic val -> addStoneToCount dic val 1) M.empty xs

addStoneToCount :: StoneCount -> Val -> Count -> StoneCount
addStoneToCount sc val count = case M.lookup val sc of
    Just x -> M.adjust (+ count) val sc
    Nothing -> M.insert val count sc

doBlink :: StoneCount -> StoneCount
doBlink sc = foldl (\sc (val, count) -> addStoneToCount sc val count) M.empty newPairs
  where
    flattenPair :: ([a], b) -> [(a, b)]
    flattenPair ([], _) = []
    flattenPair (x : xs, y) = (x, y) : flattenPair (xs, y)
    newPairs :: [(Val, Count)]
    newPairs = [(doIteration [val], count) | (val, count) <- M.toList sc] >>= flattenPair

repeatAction :: (a -> a) -> a -> Int -> a
repeatAction action acc 0 = acc
repeatAction action acc n = repeatAction action (action acc) (n - 1)

sumStones :: StoneCount -> Int
sumStones sc = sum $ M.elems sc

main :: IO ()
main = do
    contents <- readFile fileName
    let parsedInput = parseInput contents
    print ("P1: " ++ show (sumStones $ repeatAction doBlink (makeStoneCounts parsedInput) 25))
    print ("P2: " ++ show (sumStones $ repeatAction doBlink (makeStoneCounts parsedInput) 75))
  where
    fileName :: String
    -- fileName = "test.txt"
    fileName = "input.txt"
