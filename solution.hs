module Solution (solve) where

solve:: [String] -> [(Integer, Integer)]
solve [] = []
solve (expr:rest)
    | expr == "END" = []
    | otherwise = solve rest


main::IO()
main = putStr =<< (return . writeOutput show. solve . readInput lines) =<< getContents

writeOutput::(a -> String) -> [a] -> String
writeOutput fmt = unlines . (map fmt)

readInput::Read b => (String -> [String]) -> String -> [b]
readInput mince = (map read) . mince
