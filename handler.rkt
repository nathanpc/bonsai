(module handler racket
  (provide accept-and-handle)
  (provide handle)
  
  (require "./while.rkt")
  
  
  ; Parse request headers
  (define (get-header in)
    (define tmp-list (list))

    (define current-header (read-line in))
    (while (not (equal? current-header "\r"))
      (set! tmp-list (append tmp-list (string-split current-header "\r")))
      (set! current-header (read-line in)))
    tmp-list)
  
  (define (get-file-location headers)
    (define loc-string (list-ref (string-split (list-ref headers 0)) 1))
    (define loc-list (string-split loc-string "/"))
    (define loc-string-last (string-ref loc-string (- (string-length loc-string) 1)))
    
    (cond
      [(directory-exists? (format "./htdocs~a" loc-string))
        (set! loc-string (format "/~a/~a" (string-join loc-list "/") "index.html"))])
    
    (format "./htdocs~a" loc-string))
  
  ; Setup the connection handler.
  (define (accept-and-handle listener)
    (define-values (in out) (tcp-accept listener))
    (handle in out)
    (close-input-port in)
    (close-output-port out))
  
  ; The handler.
  (define (handle in out)
    (define headers (get-header in))

    ;(display (format "~s~n" headers))
    (display (format "~a~n" (list-ref headers 0)))

    (define file (get-file-location headers))
    (display (format "~a~n~n" file))
    
    (cond ((file-exists? file)
      (display "HTTP/1.0 200 OK\r\n" out)
      (display "Server: bamboo v0.0.0a\r\n" out)
      (display "Content-Type: text/html\r\n" out)
      (display "\r\n" out)
      
      (display (file->string file #:mode 'text) out)
      (display "\r\n" out))
    (else
      (display "HTTP/1.0 404 Not Found\r\n" out)
      (display "Server: bamboo v0.0.0a\r\n" out)
      (display "Content-Type: text/html\r\n" out)
      (display "\r\n" out)

      (display "<h1>Not Found</h1>\r\n" out)))))