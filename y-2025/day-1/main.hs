data Movement = L Int | R Int
  deriving (Show, Eq)

instance Read Movement where
  readsPrec :: Int -> ReadS Movement
  readsPrec _ input = let (dir:num) = input in
    case dir of
      'L' -> [(L (read num :: Int), "")]
      'R' -> [(R (read num :: Int), "")]

rot :: Int -> Movement -> (Int, Int)
rot pos (R move) =  let (t , r ) = (move `divMod` 100) 
                        (t', r') = ((pos + r) `divMod` 100) 
                    in  (t + t', r')

-- symmetrical operation around 0/100
rot pos (L move) =  let (t , r ) = (move `divMod` 100) 
                        pos' = (-pos) `mod` 100
                        (t', r') = ((pos' + r) `divMod` 100)
                    in  (t + t', (-r') `mod` 100 )


p1 :: Int -> [Movement] -> Int
p1 init moves = snd $ partial init moves where
  partial :: Int -> [Movement] -> (Int, Int)
  partial init moves = foldl fn (init, 0) moves
  fn :: (Int, Int) -> Movement -> (Int, Int)
  fn (val, zs) move = let newVal = snd $ rot val move in
    case newVal of
      0 -> (newVal, zs+1)
      _ -> (newVal, zs)

p2 :: Int -> [Movement] -> Int
p2 init moves = snd $ partial init moves where
  partial :: Int -> [Movement] -> (Int, Int)
  partial init moves = foldl fn (init, 0) moves
  fn :: (Int, Int) -> Movement -> (Int, Int)
  fn (val, zs) move = let (add, newVal) = rot val move in (newVal, zs + add)

-- ghc -no-keep-hi-file -no-keep-o-files main.hs
main :: IO ()
main = do
    contents <- readFile fileName
    let fileLines = lines contents
    let movements :: [Movement] = map read fileLines
    print ("P1: " ++ show (p1 50 movements))
    print ("P2: " ++ show (p2 50 movements))
  where
    fileName :: String
    -- fileName = "test.txt"
    fileName = "input.txt"
