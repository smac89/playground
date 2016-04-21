mergeSort::Ord a => [a] -> [a]
mergeSort l = mergeSort' $ map (\x -> [x]) l where
   mergeSort' [] = []
   mergeSort' (l:[]) = l
   mergeSort' (l:r:xs) = merge (merge l r) (mergeSort' xs) where
      merge left right | null left = right
                       | null right = left
                       | x <= y = x : merge (tail left) right
                       | otherwise = y : merge left (tail right) where
                        x = head left
                        y = head right


bubbleSort::Ord a => [a] -> [a]
bubbleSort l = let (list, sorted) = bubbleSort' l
               in if sorted then list
                  else bubbleSort list where
                     bubbleSort' [] = ([], True)
                     bubbleSort' [x] = ([x], True)
                     bubbleSort' (x:y:xs) | x > y = (fst (bubbleSort' (y:x:xs)), False)
                                          | otherwise = (x:l, True && s) where
                                             (l, s) = bubbleSort' (y:xs)