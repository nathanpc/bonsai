#lang racket

(define server-custodian (make-custodian))

(define start-server
  (lambda ([port 8080])
    (current-custodian server-custodian)
      ; Caps at 50 connections at the same time
      (define listener (tcp-listen port 50 #t))
      (define (loop)
        (accept-and-handle listener)
        (loop))
      (define t (thread loop))
      (lambda ()
        (kill-thread t)
        (tcp-close listener))
    (fprintf (current-output-port) "Server started listening at ~a" port)))

(define stop-server
  (lambda ()
    (custodian-shutdown-all server-custodian)
    (display "Server terminated")))

(define bonsai
  (lambda (command [port 8080])
    (cond ((equal? command "start")
           (start-server port))
         (else
          (stop-server)))))

(define (accept-and-handle listener)
  (define-values (in out) (tcp-accept listener))
  (handle in out)
  (close-input-port in)
  (close-output-port out))

(define (handle in out)
  ; Discart the request header (up to a blank line)
  (regexp-match #rx"(\r\n|^)\r\n" in)
  ; Send reply
  (display "HTTP/1.0 200 OK\r\n" out)
  (display "Server: bamboo v0.0.0a\r\n" out)
  (display "Content-Type: text/html\r\n" out)
  (display "\r\n" out)
  (display "<h1>It works!</h1>" out))


(bonsai "start")
