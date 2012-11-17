(module handler racket
  (provide accept-and-handle)
  (provide handle)
  
  (require "./while.rkt")
  
  (define (get-header in)
    (define tmp-list (list))

    (define current-header (read-line in))
    (while (not (equal? current-header "\r"))
      (set! tmp-list (append tmp-list (string-split current-header "\r")))
      (set! current-header (read-line in)))
    tmp-list)


  (define (request-type req-head)
    (list-ref (string-split req-head) 0))
  
  (define (requested-location req-head)
    (list-ref (string-split req-head) 1))
  
  ; Setup the connection handler.
  (define (accept-and-handle listener)
    (define-values (in out) (tcp-accept listener))
    (handle in out)
    (close-input-port in)
    (close-output-port out))
  
  ; The handler.
  (define (handle in out)
    (define headers (get-header in))

    (display (format "~s~n" headers))
    (display (format "~s~n" (string-split (list-ref headers 0))))
    
    ; Discart the request header (up to a blank line)
    ;(regexp-match #rx"(\r\n|^)\r\n" in)

    ; Send reply
    (display "HTTP/1.0 200 OK\r\n" out)
    (display "Server: bamboo v0.0.0a\r\n" out)
    (display "Content-Type: text/html\r\n" out)
    (display "\r\n" out)
    ;(file->string file #:mode 'text)
    (display "<h1>It works!</h1>\r\n\r\n" out)))