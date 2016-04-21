import Data.Char (digitToInt)

main :: IO ()
main = do
	numbers <- putStr "How many happy numbers to find? " >> readLn :: IO Int
	print $ happy numbers

happy :: Int -> [Integer]
happy = (`take` (filter isHappy [1..]))
	where isHappy n | n == 4 = False
					| n == 1 = True
			  		| otherwise = isHappy $ reduce n
			  		where reduce = sum . (map $ (^ 2).toInteger.digitToInt) . show
