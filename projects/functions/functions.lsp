;;these are comments. 

(defun dist-from-orig (x y &optional (z 0))
    "computes 3-d distance from origin"
    (sqrt(+ (* x x) (* y y) (* z z))))

(defun fac (n)
    (cond ((<= n 1) 1)
            (t (* n (fac (1- n))))))
            
(defun drop-val (atm lst)
    (cond ((atom lst) lst)
        ((equal (car lst) atm) (drop-val atm (cdr lst)))
        (t (cons (car lst) (drop-val atm (cdr lst))))))

(defun sum-even (lst)
    "calculates the sum of even numbers inside lists, recursively"
    (cond 
        ((null lst) 0)                                     ;; empty list?
        ((numberp (car lst))                               ;; checks 1st atom
        (if (evenp (car lst))                              ;; checks 1st number
            (+ (car lst) (sum-even (cdr lst)))             ;; adds 1st even, calls function, removes 1st atom (i++)
            (sum-even (cdr lst))))                         ;; else, removes 1st atom (i++)
        ((listp (car lst))                                 ;; checks if list
            (+ (sum-even (car lst)) (sum-even (cdr lst)))) ;; adds 1st even of [nested] list, calls function, removes 1st atom of [nested] list
        (t (sum-even (cdr lst)))))                         ;; else, removes 1st atom in list
