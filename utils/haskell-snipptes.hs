{-# LANGUAGE CPP #-}
import qualified Data.Text as T

getPathFromRelative :: String -> String
getPathFromRelative file = parentFolder ++ file
    where
        thisFilePath :: String
        thisFilePath = T.unpack (T.replace (T.pack "\\\\") (T.pack "\\") (T.pack __FILE__))
        parentFolder :: String
        parentFolder = T.unpack (T.replace (last (T.splitOn (T.pack "\\") (T.pack thisFilePath))) (T.pack "") (T.pack thisFilePath))
