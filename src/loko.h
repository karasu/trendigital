/**************************************************************************
    This file is part of Tren Digital.

    Tren Digital is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tren Digital is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tren Digital.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/
#ifndef LOKO_H
#define LOKO_H

#include <QString>
#include <QList>
#include <QDataStream>

class LokoDock;

class Loko
{
public:
    Loko();

    inline QString getImageFileName() { return m_imageFileName; }
    inline void setImageFileName(QString fileName) { m_imageFileName = fileName; }

    bool load(QDataStream & ar, int fileVersion);
    bool save(QDataStream & ar);

    void storeLocomotive(void);

    void clear(void);

    void copy(Loko *pLoko);

    LokoDock * getLokoDock(void);

    QString m_name;
    QString m_type;
    QString m_description;
    QString m_brand; // marca
    QString m_engine; // tipus de motor
    QString m_imageFileName;

    int m_id;
    int m_address;
    int m_extraDecoder;

    bool m_direction;
    bool m_functions[NUM_FUNCTIONS];
    bool m_extraFunctions[NUM_EXTRA_FUNCTIONS];

    bool m_dockVisible;

    bool m_multitracked;
    bool m_multitrack;
    QList <int> m_multitrack_ids;

    // Guardem l'estat de les variables

    bool m_stop;
    int m_speed;
    bool m_light;

    int m_interfaceId;

    // Mètodes

    void setLight(bool lightOn);


/*
            void SetCurrentSpeed(int iSpeed);
            void SetCurrentStop(BOOL bCurrentStop);
            void SetReverse(void);

            BOOL Serialize(CArchive & ar, int iVersion);

            void SetFunctions(BOOL bFunctions[NUM_FUNCTIONS]);
            void SetAditionalFunctions(BOOL bAditionalFunctions[NUM_ADITIONAL_FUNCTIONS]);

            void SetLight(BOOL bLight);

            CLocomotiveControlDlg * GetLocomotiveControlDlg(void);

            void UpdateLocomotiveControlLight(BOOL bLight);
            void UpdateLocomotiveControlStop(BOOL bStop);
            void UpdateLocomotiveControlSpeed(int iSpeed);
            void UpdateLocomotiveControlDirection(int iDirection);

            void UpdateFunctions(BOOL bFunctions[NUM_FUNCTIONS]);
            void UpdateAditionalFunctions(BOOL bAditionalFunctions[NUM_ADITIONAL_FUNCTIONS]);

            CLocomotive * GetLocomotiveByID(int iID);

            // --------------------------------------------------------------------------------------------------------

            BOOL IsMultitracked() { return m_bMultitracked; }

            BOOL IsMultitrackType() { return m_bMultitrack; }

            void SetMultitrackType(BOOL bStatus) { m_bMultitrack = bStatus; }

            void SetMultitracked(BOOL bStatus) { m_bMultitracked = bStatus; }

            void AddMTLocomotive(int iID);
            void RemoveAllMTLocomotives(void);
            void RemoveMTLocomotive(int iID);
            void CopyDataFromFirstMTLocomotive(void);

            void SetMTCurrentSpeed(int iSpeed);
            void SetMTCurrentStop(BOOL bCurrentStop);
            void SetMTReverse(void);
            void SetMTLight(BOOL bLight);

            void SetMTFunctions(BOOL bFunctions[NUM_FUNCTIONS]);
            void SetMTAditionalFunctions(BOOL bAditionalFunctions[NUM_ADITIONAL_FUNCTIONS]);

            void UpdateMTFunctions(BOOL bFunctions[NUM_FUNCTIONS]);
            void UpdateMTAditionalFunctions(BOOL bAditionalFunctions[NUM_ADITIONAL_FUNCTIONS]);

            void UpdateMTLocomotiveControlLight(BOOL bLight);
            void UpdateMTLocomotiveControlSpeed(int iSpeed);
            void UpdateMTLocomotiveControlStop(BOOL bStop);

            // --------------------------------------------------------------------------------------------------------

            void StoreLocomotive(void); // Special for new TCP/IP stations
        public:
                std::deque <int> m_iMTLocomotiveIDs;   multitrack_ids

        protected:
                CString m_strName;
                CString m_strType;
                CString m_strDescription;
                CString m_strModel;
                CString m_strImageFileName;

                int m_iID;
                int m_iAddress;
                int m_iAditionalDecoder;

                BOOL m_bDirection;
                BOOL m_bFunctions[NUM_FUNCTIONS];
                BOOL m_bAditionalFunctions[NUM_ADITIONAL_FUNCTIONS];

                CRect m_controlDlgRect;
                BOOL m_bControlDlgVisible;

                BOOL m_bMultitracked;
                BOOL m_bMultitrack;

                // Guardem l'estat de les variables

                BOOL	m_bCurrentStop;
                int		m_iCurrentSpeed;
                BOOL	m_bLight;
                BOOL	m_bCurrentFunctions[NUM_FUNCTIONS];
                BOOL	m_bCurrentAditionalFunctions[NUM_ADITIONAL_FUNCTIONS];

                // ID for networked interfaces (do not save it to trk file!)
                int m_iInterfaceID;
                */
};

#endif // LOKO_H
