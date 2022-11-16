#include "config.h"
#include <inttypes.h>
#include <math.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>

#include "libavutil/avstring.h"
#include "libavutil/channel_layout.h"
#include "libavutil/eval.h"
#include "libavutil/mathematics.h"
#include "libavutil/pixdesc.h"
#include "libavutil/imgutils.h"
#include "libavutil/dict.h"
#include "libavutil/fifo.h"
#include "libavutil/parseutils.h"
#include "libavutil/samplefmt.h"
#include "libavutil/time.h"
#include "libavutil/bprint.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"
#include "libswscale/swscale.h"
#include "libavutil/opt.h"
#include "libavcodec/avfft.h"
#include "libswresample/swresample.h"

#if CONFIG_AVFILTER
# include "libavfilter/avfilter.h"
# include "libavfilter/buffersink.h"
# include "libavfilter/buffersrc.h"
#endif

#include "cmdutils.h"

const char program_name[] = "ffmpeg_principle";
const int program_birth_year = 2003;

void show_help_default(const char *opt, const char *arg)
{
}

static const OptionDef options[] = {
    CMDUTILS_COMMON_OPTIONS
};

/* Called from the main */
int main(int argc, char **argv)
{
    av_log_set_level(AV_LOG_TRACE);
    av_log_set_flags(AV_LOG_SKIP_REPEATED);

    parse_loglevel(argc, argv, options);

    av_log(NULL, AV_LOG_TRACE, "start ffmpeg principle\n");

    return 0;
}
