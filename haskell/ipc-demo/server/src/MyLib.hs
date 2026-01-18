{-# LANGUAGE OverloadedStrings #-}

module MyLib where

import Control.Exception (bracket, finally)
import Control.Monad (forever, void, when)
import qualified Data.ByteString.Char8 as B
import Network.Socket
  ( Family (AF_UNIX),
    SockAddr (SockAddrUnix),
    Socket,
    SocketOption (ReuseAddr),
    SocketType (Stream),
    accept,
    bind,
    close,
    defaultProtocol,
    listen,
    setSocketOption,
    socket,
    withSocketsDo,
  )
import Network.Socket.ByteString (recv, sendAll)
import System.Directory (doesFileExist, removeFile)

socketPath :: FilePath
socketPath = "../ipc.sock"

someFunc :: IO ()
someFunc = withSocketsDo $ do
  stale <- doesFileExist socketPath
  when stale $ removeFile socketPath

  let addr = SockAddrUnix socketPath

  bracket (socket AF_UNIX Stream defaultProtocol) close $ \sock -> do
    setSocketOption sock ReuseAddr 1
    bind sock addr
    listen sock 16

    putStrLn "server listening on ./ipc.sock"

    putStrLn "server listening on ./ipc.sock"

    forever $ do
      (conn, _peer) <- accept sock
      void $ handdleClient conn

handdleClient :: Socket -> IO ()
handdleClient conn =
  finally
    ( do
        -- Read "some bytes". We'll treat it as one line-ish message like the C version.
        msg <- recv conn 1024
        if B.null msg
          then pure ()
          else do
            let out = B.concat ["ack: ", msg]
            sendAll conn out
    )
    (close conn)
