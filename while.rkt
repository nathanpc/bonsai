(module while racket
  (provide while)
  
  ; Credits: http://codeimmersion.i3ci.hampshire.edu/2009/10/15/a-scheme-while-loop/
  (define-syntax (while stx)
    (syntax-case stx ()
        ((_ condition expression ...)
         #`(do ()
             ((not condition))
             expression
            ...)))))