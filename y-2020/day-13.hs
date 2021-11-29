import Data.List
import Data.Ord

leavingTime :: Int
leavingTime = 1000391

buses :: [String]
buses = [ "19","x","x","x","x","x","x","x","x","x","x","x","x","37","x","x","x","x","x","383","x","x","x","x","x","x","x","23","x","x","x","x","13","x","x","x","x","x","x","x","x","x","x","x","x","x","x","x","29","x","457","x","x","x","x","x","x","x","x","x","41","x","x","x","x","x","x","17" ]

busesTest :: [String]
busesTest = ["7","13","x","x","59","x","31","19"]

getActiveBuses :: [String] -> [Int]
getActiveBuses [] = []
getActiveBuses ("x":tl) = getActiveBuses tl
getActiveBuses (i:tl) = (read i :: Int) : getActiveBuses tl

missedBusBy :: Int -> Int -> Int
missedBusBy currentTime bussId =  currentTime `mod` bussId

nextBussIn :: Int -> Int -> Int
nextBussIn currentTime bussId = bussId - missedBusBy currentTime bussId

sol1 :: Int
sol1 = uncurry (*) closestBus where
    busesAndWaitTimes = [(waitTime, bus) | bus <- getActiveBuses buses, let waitTime = nextBussIn leavingTime bus]
    closestBus = head (sortOn fst busesAndWaitTimes)

getBusesWithDelay :: [String] -> [(Int, Int )]
getBusesWithDelay list = getBusesWithDelayTLR list 0 [] where
    getBusesWithDelayTLR:: [String] -> Int -> [(Int,Int)] -> [(Int,Int)]
    getBusesWithDelayTLR [] accN accL = accL
    getBusesWithDelayTLR ("x":tl) accN accL = getBusesWithDelayTLR tl (accN+1) accL
    getBusesWithDelayTLR (id:tl) accN accL = getBusesWithDelayTLR tl (accN+1) (accL++[(read id :: Int, accN)])

-- Notice all buses have prime IDs => Chinese remainder theorem (https://en.wikipedia.org/wiki/Chinese_remainder_theorem#Computation)
-- m = multiple
-- r = remainder
sol2 :: [String] -> (Int, Int)
sol2 busList = foldl1 sieveIteration (toMulAndRemainder localBuses)
    where
        localBuses :: [(Int,Int)]
        localBuses = sortOn (Data.Ord.Down . fst) (getBusesWithDelay busList)

        toMulAndRemainder :: [(Int, Int)] -> [(Int, Int)]
        toMulAndRemainder bs = [(id, (-delay) `mod` id) | (id, delay) <- bs]
        
        listOfCandidates :: Int -> Int -> [Int]
        listOfCandidates m r = [i * m + r | i <- [0..]]

        nextSieveNumber :: Int -> Int -> Int -> Int -> Int
        nextSieveNumber m r nm nr = (head . filter (\num -> num `mod` nm == nr)) (listOfCandidates m r)

        sieveIteration :: (Int, Int) -> (Int, Int) -> (Int,Int)
        sieveIteration (accMul, accRem) (newMul, newRem) = (lcm accMul newMul, nextSieveNumber accMul accRem newMul newRem)




main :: IO()
main = do
    print sol1
    -- print (getBusesWithDelay buses)
    -- print (uncurry (zipWith (\ id delay -> (id - delay) `mod` id)) ( unzip (getBusesWithDelay buses)))
    -- print (uncurry (zipWith (\ id delay -> (-delay) `mod` id)) ( unzip (getBusesWithDelay buses)))
    -- print (sol2 busesTest)
    print ((snd . sol2) buses)