(ns parattice.example.main
  (:import parattice.PaRattice
           parattice.Lattice
           parattice.LatticeKMP))

(def paradict [[["blood" "stem" "cell"] ["造血" "幹" "細胞"] ["hematopoietic" "stem" "cell"]]
               [["造血" "幹" "細胞" "移植"] ["hematopoietic" "stem" "cell" "transplantation"]]
               [["stem" "cell"] ["幹" "細胞"]]
               [["幹" "細胞" "移植"] ["rescue" "transplant"] ["stem" "cell" "rescue"]]
               [["rescue"] ["救命"]]
               [["blood"] ["血液"]]])
(def words ["造血" "幹" "細胞" "移植"])

(defn -main
  [& args]
  (with-open [;; initialization
              parattice (PaRattice. paradict)
              ;; lattice generation
              lattice (.getLattice parattice words true 2)]
    ;; dump a generated lattice
    (spit "paraphrase-lattice.dot" (.dumpDot lattice true))
    ;; serialization
    (let [b (.toBytes lattice)
          results (with-open [;; deserialization
                              new-lattice (Lattice/fromBytes b)
                              ;; search
                              kmp (LatticeKMP. ["幹" "細胞"])]
                    (.search kmp new-lattice))]
      (doseq [result results]
        (doseq [edge result]
          (print (str "(" (.first edge) ", " (.second edge) ") ")))
        (println)
        (doseq [edge (.getTrunkSpan lattice result)]
          (print (str "(" (.first edge) ", " (.second edge) ") ")))
        (println "\n===========")))))
