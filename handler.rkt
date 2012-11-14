(module handler racket
  (provide accept-and-handle)
  (provide handle)
  
  ; Setup the connection handler.
  (define (accept-and-handle listener)
    (define-values (in out) (tcp-accept listener))
    (handle in out)
    (close-input-port in)
    (close-output-port out))
  
  ; The handler.
  (define (handle in out)
    (display in)
    ;(define request-headers (port->string in))
    ;(sync )
    ;(print (handle-evt? (eof-evt in)))
    ;(print (evt? (eof-evt in)))
    ;(display (format "~a~n" ))
    ; Discart the request header (up to a blank line)
    ;(regexp-match #rx"(\r\n|^)\r\n" in)

    ; Send reply
    (display "HTTP/1.0 200 OK\r\n" out)
    (display "Server: bamboo v0.0.0a\r\n" out)
    (display "Content-Type: text/html\r\n" out)
    (display "\r\n" out)
    ;(file->string file #:mode 'text)
    (display "<h1>It works!</h1>" out)))