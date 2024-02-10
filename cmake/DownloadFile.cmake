function(DownloadFile URL OutputFile HASH)
  message(STATUS "Download \"${URL}\" to \"${OutputFile}\"")
  # ファイルをダウンロードする．
  # ダウンロードの結果はResultに格納される．
  file(
    DOWNLOAD ${URL} ${OutputFile} EXPECTED_HASH SHA256=${HASH} STATUS Result
  )
  # 結果を取り出す
  list(GET Result 0 Succeeded)
  list(GET Result 1 ErrorDesc)
  # ダウンロードが成功したかを調べる
  if (${Succeeded} EQUAL 0)
    message(STATUS "Download succeeded: ${ErrorDesc}")
    return()
  endif()
  message(FATAL_ERROR "Download failed: ${ErrorDesc}")
endfunction(DownloadFile)
