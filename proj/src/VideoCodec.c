#include <lcom/lcf.h>
#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include "graphic.h"
#include <lcom/timer.h>
#include "keyboard.h"
#include "VideoCodec.h"
#define FPS 24

int videoCodec(){

    AVFormatContext * pFormatCtx = NULL; 

    int ret = avformat_open_input(&pFormatCtx, "img/fireflies.mp4", NULL, NULL);   
    if (ret < 0)
    {
        // couldn't open file
        printf("Could not open file %s\n", "img/fireflies.mp4");

        // exit with error
        return -1;
    }

    ret = avformat_find_stream_info(pFormatCtx, NULL);  
    if (ret < 0)
    {
        // couldn't find stream information
        printf("Could not find stream information %s\n", "img/fireflies.mp4");

        // exit with error
        return -1;
    }


    av_dump_format(pFormatCtx, 0, "img/fireflies.mp4", 0);

    unsigned int i;


    AVCodecContext * pCodecCtxOrig = NULL;
    AVCodecContext * pCodecCtx = NULL;

    // Find the first video stream
    int videoStream = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
    {
        // check the General type of the encoded data to match
    // AVMEDIA_TYPE_VIDEO
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }

    if (videoStream == -1)
    {
        // didn't find a video stream
        return -1;
    }


    // AVStream::codec deprecated
    // https://ffmpeg.org/pipermail/libav-user/2016-October/009801.html
    // pCodecCtxOrig = pFormatCtx->streams[videoStream]->codec;

    // But we still have to find the actual codec and open it:
    const AVCodec * pCodec = NULL;

    // Find the decoder for the video stream
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id); 
    if (pCodec == NULL)
    {
        // codec not found
        printf("Unsupported codec!\n");

        // exit with error
        return -1;
    }

    pCodecCtxOrig = avcodec_alloc_context3(pCodec); // [7]
    ret = avcodec_parameters_to_context(pCodecCtxOrig, pFormatCtx->streams[videoStream]->codecpar);


    // Copy context
    // avcodec_copy_context deprecation
    // http://ffmpeg.org/pipermail/libav-user/2017-September/010615.html
    //ret = avcodec_copy_context(pCodecCtx, pCodecCtxOrig);
    pCodecCtx = avcodec_alloc_context3(pCodec); // [7]
    ret = avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
    if (ret != 0)
    {
        // error copying codec context
        printf("Could not copy codec context.\n");

        // exit with error
        return -1;
    }

    // Open codec
    ret = avcodec_open2(pCodecCtx, pCodec, NULL);   // [8]
    if (ret < 0)
    {
        // Could not open codec
        printf("Could not open codec.\n");

        // exit with error
        return -1;
    }

    // Now we need a place to actually store the frame:
    AVFrame * pFrame = NULL;

    // Allocate video frame
    pFrame = av_frame_alloc();  // [9]
    if (pFrame == NULL)
    {
        // Could not allocate frame
        printf("Could not allocate frame.\n");

        // exit with error
        return -1;
    }



    // Allocate an AVFrame structure
    AVFrame * pFrameRGB = NULL;
    pFrameRGB = av_frame_alloc();
    if (pFrameRGB == NULL)
    {
        // Could not allocate frame
        printf("Could not allocate frame.\n");

        // exit with error
        return -1;
    }

    // Even though we've allocated the frame, we still need a place to put
    // the raw data when we convert it. We use avpicture_get_size to get
    // the size we need, and allocate the space manually:
    uint8_t * buffer = NULL;
    int numBytes;

    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height, 32); // [10]
    buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));    // [11]


    av_image_fill_arrays( // [12]
        pFrameRGB->data,
        pFrameRGB->linesize,
        buffer,
        AV_PIX_FMT_RGB24,
        pCodecCtx->width,
        pCodecCtx->height,
        32
    );



    struct SwsContext * sws_ctx = NULL;

    AVPacket * pPacket = av_packet_alloc();
    if (pPacket == NULL)
    {
        // couldn't allocate packet
        printf("Could not alloc packet,\n");

        // exit with error
        return -1;
    }

    // initialize SWS context for software scaling
    sws_ctx = sws_getContext(   // [13]
        pCodecCtx->width,
        pCodecCtx->height,
        pCodecCtx->pix_fmt,
        //pCodecCtx->width,
        //pCodecCtx->height,
        640,
        360,
        AV_PIX_FMT_RGB24,   // sws_scale destination color scheme
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );


    int ipc_status, r;
    uint8_t timer_bit_no;
    message msg;
    if (timer_subscribe_int(&timer_bit_no) != 0) return 1;

    i = 0;
    bool isPaused = false;
    while (av_read_frame(pFormatCtx, pPacket) >= 0) 
    {
        // Is this a packet from the video stream?
        if (pPacket->stream_index == videoStream)
        {
            // Decode video frame
            // avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &pPacket);
            // Deprecated: Use avcodec_send_packet() and avcodec_receive_frame().
            ret = avcodec_send_packet(pCodecCtx, pPacket); 
            {
                // could not send packet for decoding
                printf("Error sending packet for decoding.\n");

                // exit with eror
                return -1;
            }

            while (ret >= 0)
            {
                ret = avcodec_receive_frame(pCodecCtx, pFrame); 

                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    // EOF exit loop
                    break;
                }
                else if (ret < 0)
                {
                    // could not decode packet
                    printf("Error while decoding.\n");

                    // exit with error
                    return -1;
                }

                // Convert the image from its native format to RGB
                sws_scale( 
                    sws_ctx,
                    (uint8_t const * const *)pFrame->data,
                    pFrame->linesize,
                    0,
                    pCodecCtx->height,
                    pFrameRGB->data,
                    pFrameRGB->linesize
                );
                uint32_t irq_set = BIT(timer_bit_no);
                float ticks = 1/576 * FPS;
                // interrupt loop
                while (ticks > 0) {
                    // Get a request message.
                    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
                        printf("driver_receive failed with: %d", r);
                        continue;
                    }
                    if (is_ipc_notify(ipc_status)) { // received notification
                        switch (_ENDPOINT_P(msg.m_source)) {
                        case HARDWARE:
                        if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
                            if (ticks == 0) {
                                draw_frame(80, 0, 640, 360, pFrameRGB->data[0]);
                            }
                            ticks--;
                        }
                        break;
                        default:
                        break;
                        }
                    }
                }
            }
        }
        av_packet_unref(pPacket);
        //pause the video
        if(kbd_scan_space() == 0){
            isPaused = true;
        }
        while(isPaused == true){
            if(kbd_scan_space() == 0){
                isPaused = false;
            }
        }
        //break while loop stops the video
        if(kbd_scan_esc() == 0){
            break;
        }

    }

    if (timer_unsubscribe_int() != 0) return 1;

    /**
     * Cleanup.
     */

    // Free the RGB image
    av_free(buffer);
    av_frame_free(&pFrameRGB);
    av_free(pFrameRGB);

    // Free the YUV frame
    av_frame_free(&pFrame);
    av_free(pFrame);

    // Close the codecs
    avcodec_close(pCodecCtx);
    avcodec_close(pCodecCtxOrig);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
}

