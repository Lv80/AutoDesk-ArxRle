; MODULE_ID ACAD2007_LSP_
;;;    ACAD2016.LSP Version 1.0 for AutoCAD 2016
;;;
;;;  Copyright 2015 Autodesk, Inc.  All rights reserved.
;;;
;;;  Use of this software is subject to the terms of the Autodesk license 
;;;  agreement provided at the time of installation or download, or which 
;;;  otherwise accompanies this software in either electronic or hard copy form.
;;;
;;;
;;;
;;;    Note:
;;;            This file is normally loaded only once per AutoCAD session.
;;;            If you wish to have LISP code loaded into every document,
;;;            you should add your code to acaddoc.lsp.
;;;
;;;    Globalization Note:   
;;;            We do not support autoloading applications by the native 
;;;            language command call (e.g. with the leading underscore
;;;            mechanism.)

(if (not (=  (substr (ver) 1 11) "Visual LISP")) (load "acad2016doc.lsp"))
(arxload "ArxRle.arx")

;; Silent load.
(princ)


;;;-----BEGIN-SIGNATURE-----
;;; IwgAADCCCB8GCSqGSIb3DQEHAqCCCBAwgggMAgEBMQ8wDQYJKoZIhvcNAQEFBQAw
;;; CwYJKoZIhvcNAQcBoIIFiTCCBYUwggRtoAMCAQICECnBWz+qzVJqTme9PE5+P/Iw
;;; DQYJKoZIhvcNAQEFBQAwgbQxCzAJBgNVBAYTAlVTMRcwFQYDVQQKEw5WZXJpU2ln
;;; biwgSW5jLjEfMB0GA1UECxMWVmVyaVNpZ24gVHJ1c3QgTmV0d29yazE7MDkGA1UE
;;; CxMyVGVybXMgb2YgdXNlIGF0IGh0dHBzOi8vd3d3LnZlcmlzaWduLmNvbS9ycGEg
;;; KGMpMTAxLjAsBgNVBAMTJVZlcmlTaWduIENsYXNzIDMgQ29kZSBTaWduaW5nIDIw
;;; MTAgQ0EwHhcNMTIwNzI1MDAwMDAwWhcNMTUwOTIwMjM1OTU5WjCByDELMAkGA1UE
;;; BhMCVVMxEzARBgNVBAgTCkNhbGlmb3JuaWExEzARBgNVBAcTClNhbiBSYWZhZWwx
;;; FjAUBgNVBAoUDUF1dG9kZXNrLCBJbmMxPjA8BgNVBAsTNURpZ2l0YWwgSUQgQ2xh
;;; c3MgMyAtIE1pY3Jvc29mdCBTb2Z0d2FyZSBWYWxpZGF0aW9uIHYyMR8wHQYDVQQL
;;; FBZEZXNpZ24gU29sdXRpb25zIEdyb3VwMRYwFAYDVQQDFA1BdXRvZGVzaywgSW5j
;;; MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAqGJg65ndBqvHs0rA5X4G
;;; iRRBaZTTzYVszNrhUGmAAf4IKUNdfjeAemqPk6qzSFgyKrdySoWlPPPZ8Zf+7Xlh
;;; sLjrq7LSNmdGxA4V4l2pv24nCth1S8p7ZYkPurU/p5YHzfLYAdjczNAaRNWAp1Nm
;;; +g8EMOFewVfvxf//N8hhTqXj5bps18TcPRClpGqvNbJZpk8X+1MWYD/Txmy8PICw
;;; D5OD0ySPe/uQQaoZC29WKkn1p9zzTH7DSocP1cADdHUSnjOh/EpDnc/qLK/jch/O
;;; pbCkLonLOH8CubhUh0B7CLemdLalr5op0anHlIvboEZRq8ofV9Wagqny/4IHc2Gt
;;; 2QIDAQABo4IBezCCAXcwCQYDVR0TBAIwADAOBgNVHQ8BAf8EBAMCB4AwQAYDVR0f
;;; BDkwNzA1oDOgMYYvaHR0cDovL2NzYzMtMjAxMC1jcmwudmVyaXNpZ24uY29tL0NT
;;; QzMtMjAxMC5jcmwwRAYDVR0gBD0wOzA5BgtghkgBhvhFAQcXAzAqMCgGCCsGAQUF
;;; BwIBFhxodHRwczovL3d3dy52ZXJpc2lnbi5jb20vcnBhMBMGA1UdJQQMMAoGCCsG
;;; AQUFBwMDMHEGCCsGAQUFBwEBBGUwYzAkBggrBgEFBQcwAYYYaHR0cDovL29jc3Au
;;; dmVyaXNpZ24uY29tMDsGCCsGAQUFBzAChi9odHRwOi8vY3NjMy0yMDEwLWFpYS52
;;; ZXJpc2lnbi5jb20vQ1NDMy0yMDEwLmNlcjAfBgNVHSMEGDAWgBTPmanqeyb0S8mO
;;; j9fwBSbv49KnnTARBglghkgBhvhCAQEEBAMCBBAwFgYKKwYBBAGCNwIBGwQIMAYB
;;; AQABAf8wDQYJKoZIhvcNAQEFBQADggEBANjpBr7omO08iOslU0AGJkzvjKThRgvD
;;; H5R0m6HyYri8a0tSl25M7ADxTz7FNaLn7RYbFxbQ0PKzE6v48LoE6WyVERFdG7hJ
;;; C5fACkYWEEygSoNSP6bgb25CaHAxNKUcLQc98UpV1xMMmD3Gwjp6zNzmeWysdUIo
;;; or4sZXBloTt8LPdOLLLrTxX+JleTw4t1NmKdR4GwSv1JvxS6+mAGHmWgPCmNQn0B
;;; IrBd1Ck8Ju9ne0vnyX/vkjhnmitJLpnoVXG2r0CUzlXm7mfVfqlNJ5NZTK6r3vQZ
;;; 0CuvQUKjWbu+7wjtMJvNXH8pwMZDmCmRt4nWOO6jyToFypMJiNvvdqMxggJaMIIC
;;; VgIBATCByTCBtDELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDlZlcmlTaWduLCBJbmMu
;;; MR8wHQYDVQQLExZWZXJpU2lnbiBUcnVzdCBOZXR3b3JrMTswOQYDVQQLEzJUZXJt
;;; cyBvZiB1c2UgYXQgaHR0cHM6Ly93d3cudmVyaXNpZ24uY29tL3JwYSAoYykxMDEu
;;; MCwGA1UEAxMlVmVyaVNpZ24gQ2xhc3MgMyBDb2RlIFNpZ25pbmcgMjAxMCBDQQIQ
;;; KcFbP6rNUmpOZ708Tn4/8jANBgkqhkiG9w0BAQUFADANBgkqhkiG9w0BAQEFAASC
;;; AQAIdN8c1W+9Bbbe3Lf0IbvRJH9+BWCTmiO8nKFnBA9ESW4PF4jc3/q4JAv4vJUU
;;; X2Ihku/tRJM6bqxiD/CObouXI6b+x3evQ0sX7CofDB5voPZiJ4D4fvMekfcOzq3G
;;; 1mWWJImckvoOPkgqfbK4k2Dmdi1Ghl8Y4b80XCr29nbYx9mFEUYHMQmsvPkaZse0
;;; W0xbE4DnGYXKXyGGWJSWb3GR4trlJyCxBWc0ToIGg94DUHnwA9VGT6IvjCie8oUr
;;; SoYkyTC3zEuycKmfWpCq3+3NF4vRLvrPXmQ+NTBSEwEmvPvsBpGlj+UrinqGKsE0
;;; oSrGT+CoSjw20D4qf1/MsvFuoWMwYQYDVR0OMVoEWDQAMAA7ADIALwA2AC8AMgAw
;;; ADEANQAvADQALwA1ADkALwAzADMALwBUAGkAbQBlACAAZgByAG8AbQAgAHQAaABp
;;; AHMAIABjAG8AbQBwAHUAdABlAHIAAAA=
;;; -----END-SIGNATURE-----