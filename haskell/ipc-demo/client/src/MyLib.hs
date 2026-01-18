{-# LANGUAGE OverloadedStrings #-}

module MyLib (someFunc) where

import qualified Data.ByteString.Char8 as B
import Network.Socket
import Network.Socket.ByteString (recv, sendAll)
import System.Environment (getArgs)

socketPath :: FilePath
socketPath = "../ipc.sock"

someFunc :: IO ()
someFunc = withSocketsDo $ do
  args <- getArgs
  let msg = case args of
        [] -> "hello\n"
        (x : _) -> B.pack x <> "\n"

  let addr = SockAddrUnix socketPath

  sock <- socket AF_UNIX Stream defaultProtocol
  connect sock addr

  sendAll sock msg

  reply <- recv sock 1024
  B.putStr reply

  close sock
