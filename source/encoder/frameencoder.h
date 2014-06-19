/*****************************************************************************
 * Copyright (C) 2013 x265 project
 *
 * Authors: Chung Shin Yee <shinyee@multicorewareinc.com>
 *          Min Chen <chenm003@163.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 * This program is also available under a commercial proprietary license.
 * For more information, contact us at license @ x265.com.
 *****************************************************************************/

#ifndef X265_FRAMEENCODER_H
#define X265_FRAMEENCODER_H

#include "common.h"
#include "wavefront.h"

#include "TLibCommon/TComPic.h"
#include "TLibCommon/TComBitStream.h"

#include "TLibEncoder/TEncCu.h"
#include "TLibEncoder/TEncSearch.h"
#include "TLibEncoder/TEncSbac.h"
#include "TLibEncoder/TEncBinCoderCABAC.h"
#include "TLibEncoder/TEncSampleAdaptiveOffset.h"

#include "framefilter.h"
#include "cturow.h"
#include "ratecontrol.h"
#include "reference.h"
#include "nal.h"

namespace x265 {
// private x265 namespace

class ThreadPool;
class Encoder;

// Manages the wave-front processing of a single encoding frame
class FrameEncoder : public WaveFront, public Thread
{
public:

    FrameEncoder();

    virtual ~FrameEncoder() {}

    void setThreadPool(ThreadPool *p);

    bool init(Encoder *top, int numRows);

    void destroy();

    void processRowEncoder(int row, const int threadId);

    void processRowFilter(int row)
    {
        m_frameFilter.processRow(row);
    }

    void enqueueRowEncoder(int row)
    {
        WaveFront::enqueueRow(row * 2 + 0);
    }

    void enqueueRowFilter(int row)
    {
        WaveFront::enqueueRow(row * 2 + 1);
    }

    void enableRowEncoder(int row)
    {
        WaveFront::enableRow(row * 2 + 0);
    }

    void enableRowFilter(int row)
    {
        WaveFront::enableRow(row * 2 + 1);
    }

    void processRow(int row, int threadId)
    {
        const int realRow = row >> 1;
        const int typeNum = row & 1;

        // TODO: use switch when more type
        if (typeNum == 0)
        {
            processRowEncoder(realRow, threadId);
        }
        else
        {
            processRowFilter(realRow);

            // NOTE: Active next row
            if (realRow != m_numRows - 1)
                enqueueRowFilter(realRow + 1);
            else
                m_completionEvent.trigger();
        }
    }

    TEncEntropy* getEntropyCoder(int row)      { return &this->m_rows[row].m_entropyCoder; }

    TEncSbac*    getSbacCoder(int row)         { return &this->m_rows[row].m_sbacCoder; }

    TEncSbac*    getRDGoOnSbacCoder(int row)   { return &this->m_rows[row].m_rdGoOnSbacCoder; }

    TEncSbac*    getBufferSBac(int row)        { return &this->m_rows[row].m_bufferSbacCoder; }

    TEncCu*      getCuEncoder(int row)         { return &this->m_rows[row].m_cuCoder; }

    /* Frame singletons, last the life of the encoder */
    TEncSampleAdaptiveOffset* getSAO()         { return &m_frameFilter.m_sao; }

    void getStreamHeaders(NALList& list, TComOutputBitstream& bs);

    void initSlice(TComPic* pic);

    /* analyze / compress frame, can be run in parallel within reference constraints */
    void compressFrame();

    /* called by compressFrame to perform wave-front compression analysis */
    void compressCTURows();

    void encodeSlice(TComOutputBitstream* substreams);

    /* blocks until worker thread is done, returns encoded picture and bitstream */
    TComPic *getEncodedPicture(NALList& list);

    void setLambda(int qp, int row);

    // worker thread
    void threadMain();

    Event                    m_enable;
    Event                    m_done;
    bool                     m_threadActive;

    int                      m_numRows;
    CTURow*                  m_rows;
    TComSPS                  m_sps;
    TComPPS                  m_pps;
    RateControlEntry         m_rce;
    SEIDecodedPictureHash    m_seiReconPictureDigest;

    uint64_t                 m_SSDY;
    uint64_t                 m_SSDU;
    uint64_t                 m_SSDV;
    double                   m_ssim;
    uint32_t                 m_ssimCnt;
    MD5Context               m_state[3];
    uint32_t                 m_crc[3];
    uint32_t                 m_checksum[3];
    double                   m_elapsedCompressTime; // elapsed time spent in worker threads
    double                   m_frameTime;           // wall time from frame start to finish

    volatile bool            m_bAllRowsStop;
    volatile int             m_vbvResetTriggerRow;

protected:

    int calcQpForCu(uint32_t cuAddr, double baseQp);
    void noiseReductionUpdate();

    Encoder*                 m_top;
    x265_param*              m_param;

    MotionReference          m_mref[2][MAX_NUM_REF + 1];
    TEncSbac                 m_sbacCoder;
    TEncBinCABAC             m_binCoderCABAC;
    FrameFilter              m_frameFilter;
    TComOutputBitstream      m_bs;
    TComOutputBitstream*     m_outStreams;
    NoiseReduction           m_nr;
    NALList                  m_nalList;

    TComPic*                 m_pic;

    int                      m_filterRowDelay;
    Event                    m_completionEvent;
    int64_t                  m_totalTime;
    bool                     m_isReferenced;
};
}

#endif // ifndef X265_FRAMEENCODER_H
