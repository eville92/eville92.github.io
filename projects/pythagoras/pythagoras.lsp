;; lisp function (Pythagorean | Pythagora's) theorem
;; finds value of hypotenuse (c)

;; pesudeocode
;; c^2 = a^2 + b^2
;; c^2 = (a * a) + (b * b) --> c^2 = (* a a) + (* b b)
;; c^2 = (+ (* a a) (b b) )
;; = (sqrt (+ (* a a) (b b) ) )

;; START

(defun pythag (a b)
    "finds the hypotenuse of a triangle"
    (sqrt (+ (* a a) (* b b))))
    
;; END
