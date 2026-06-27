INSERT INTO diagnosis (
    runny_nose, cough, strong_cough, sore_throat, fever, high_fever, 
    fatigue, diziness, nausea, headache, strong_muscle_pain, 
    trouble_concentrating, itchy_eyes, dry_mouth, diagnosis
) VALUES (
    1, 1, 1, 1, 1, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 1, 'Flu'
);
-- DROP TABLE IF EXISTS DIAGNOSIS;

-- CREATE TABLE DIAGNOSIS (
--     runny_nose INTEGER NOT NULL,
--     cough INTEGER NOT NULL,
--     strong_cough INTEGER NOT NULL,
--     sore_throat INTEGER NOT NULL,
--     fever INTEGER NOT NULL,
--     high_fever INTEGER NOT NULL,
--     fatigue INTEGER NOT NULL,
--     diziness INTEGER NOT NULL,
--     nausea INTEGER NOT NULL,
--     headache INTEGER NOT NULL,
--     strong_muscle_pain INTEGER NOT NULL,
--     trouble_concentrating INTEGER NOT NULL,
--     itchy_eyes INTEGER NOT NULL,
--     dry_mouth INTEGER NOT NULL,
--     diagnosis TEXT NOT NULL PRIMARY KEY
-- );