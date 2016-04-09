{-- |
let n be in the range 10^0 to 10^k where k >= 0
1. Find the first number h which when squared is not greater than n
2. If such a number does not exist, go to the next value of n and start at step 1
3. Determine if (n - h^2) is a perfect square; if it is, then output h and the square root of the difference
computed, then go to step 1
4. Otherwise repeat step 1 -}

main :: IO ()
main = do
	num <- putStr "How many happy numbers to find? " >> readLn :: IO Int
	happy num

happy :: Int -> IO ()
happy n | n <= 0 = putStr ""
		| otherwise = ???