#lang racket

(require "./handler.rkt")

; Setup the server custodian.
(define server-custodian (make-custodian))

; Start the server.
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
    (display (format "Server started listening at ~a~n" port))))

; Stop the server.
(define stop-server
  (lambda ()
    (custodian-shutdown-all server-custodian)
    (display "Server terminated")))

; The main initializer for bonsai.
(define bonsai
  (lambda (command [port 8080])
    (cond ((equal? command "start")
           (start-server port))
         (else
          (stop-server)))))

(define (do-not-return)
  (semaphore-wait (make-semaphore 0)))

; Start the server.
(bonsai "start")
(do-not-return)