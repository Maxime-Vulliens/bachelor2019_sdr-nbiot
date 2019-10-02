#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat Sep 28 18:40:46 2019
##################################################

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt5 import Qt
from PyQt5 import Qt, QtCore
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import lte
import nbiot
import sip
import sys
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")

        if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
            self.restoreGeometry(self.settings.value("geometry").toByteArray())
        else:
            self.restoreGeometry(self.settings.value("geometry", type=QtCore.QByteArray))

        ##################################################
        # Variables
        ##################################################
        self.subarrier_index_0 = subarrier_index_0 = 0
        self.subarrier_index = subarrier_index = 0
        self.samp_rate = samp_rate = 10000000
        self.noise_gain = noise_gain = 0.3
        self.nb_subcarrier_0 = nb_subcarrier_0 = 1
        self.nb_subcarrier = nb_subcarrier = 1
        self.nb_frame = nb_frame = 12
        self.lo_gain = lo_gain = 1
        self.lo_freq = lo_freq = -620011.0
        self.iot_samp_rate = iot_samp_rate = 1920000

        ##################################################
        # Blocks
        ##################################################
        self._noise_gain_range = Range(0, 1, 0.01, 0.2, 200)
        self._noise_gain_win = RangeWidget(self._noise_gain_range, self.set_noise_gain, 'noise_gain', "counter_slider", float)
        self.top_layout.addWidget(self._noise_gain_win)
        self._nb_frame_range = Range(1, 100, 1, 12, 20)
        self._nb_frame_win = RangeWidget(self._nb_frame_range, self.set_nb_frame, 'nb_frame', "counter_slider", int)
        self.top_layout.addWidget(self._nb_frame_win)
        self._lo_gain_range = Range(1, 100, 1, 1, 200)
        self._lo_gain_win = RangeWidget(self._lo_gain_range, self.set_lo_gain, 'lo_gain', "counter_slider", int)
        self.top_layout.addWidget(self._lo_gain_win)
        self._lo_freq_range = Range(-650000, -600000, 1, -620011.0, 20)
        self._lo_freq_win = RangeWidget(self._lo_freq_range, self.set_lo_freq, 'lo_freq', "counter_slider", float)
        self.top_layout.addWidget(self._lo_freq_win)
        self._subarrier_index_0_range = Range(0, 11, 1, 0, 200)
        self._subarrier_index_0_win = RangeWidget(self._subarrier_index_0_range, self.set_subarrier_index_0, 'subarrier_index_0', "counter_slider", int)
        self.top_layout.addWidget(self._subarrier_index_0_win)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=192,
                decimation=200,
                taps=None,
                fractional_bw=None,
        )
        self.qtgui_time_sink_x_1 = qtgui.time_sink_c(
        	1920*nb_frame, #size
        	iot_samp_rate, #samp_rate
        	"", #name
        	2 #number of inputs
        )
        self.qtgui_time_sink_x_1.set_update_time(0.10)
        self.qtgui_time_sink_x_1.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_1.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_1.enable_tags(-1, True)
        self.qtgui_time_sink_x_1.set_trigger_mode(qtgui.TRIG_MODE_TAG, qtgui.TRIG_SLOPE_POS, 1, 0.00001, 1, 'frame')
        self.qtgui_time_sink_x_1.enable_autoscale(False)
        self.qtgui_time_sink_x_1.enable_grid(True)
        self.qtgui_time_sink_x_1.enable_axis_labels(True)
        self.qtgui_time_sink_x_1.enable_control_panel(True)
        self.qtgui_time_sink_x_1.enable_stem_plot(False)

        if not True:
          self.qtgui_time_sink_x_1.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [2, 2, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(4):
            if len(labels[i]) == 0:
                if(i % 2 == 0):
                    self.qtgui_time_sink_x_1.set_line_label(i, "Re{{Data {0}}}".format(i/2))
                else:
                    self.qtgui_time_sink_x_1.set_line_label(i, "Im{{Data {0}}}".format(i/2))
            else:
                self.qtgui_time_sink_x_1.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_1.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_1.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_1.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_1.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_1.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_1_win = sip.wrapinstance(self.qtgui_time_sink_x_1.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_1_win)
        self.qtgui_time_sink_x_0_0 = qtgui.time_sink_f(
        	12*14*nb_frame, #size
        	iot_samp_rate*896/960*12/128, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0_0.set_y_axis(-1, 100)

        self.qtgui_time_sink_x_0_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0_0.set_trigger_mode(qtgui.TRIG_MODE_TAG, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, 'start_npss')
        self.qtgui_time_sink_x_0_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0_0.enable_grid(False)
        self.qtgui_time_sink_x_0_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0_0.enable_control_panel(True)
        self.qtgui_time_sink_x_0_0.enable_stem_plot(False)

        if not True:
          self.qtgui_time_sink_x_0_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_0_win)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	12*14*nb_frame, #size
        	iot_samp_rate*896/960*12/128, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_TAG, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, 'start_npss')
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(True)
        self.qtgui_time_sink_x_0.enable_stem_plot(False)

        if not True:
          self.qtgui_time_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.nbiot_remove_cp_0 = nbiot.remove_cp()
        self.nbiot_nsss_decode_0 = nbiot.nsss_decode()
        self.nbiot_npss_sync_0 = nbiot.npss_sync()
        self.nbiot_custom_fft_0 = nbiot.custom_fft()
        self._nb_subcarrier_0_range = Range(1, 12, 1, 1, 200)
        self._nb_subcarrier_0_win = RangeWidget(self._nb_subcarrier_0_range, self.set_nb_subcarrier_0, 'nb_subcarrier_0', "counter_slider", int)
        self.top_layout.addWidget(self._nb_subcarrier_0_win)
        self.low_pass_filter_0 = filter.fir_filter_ccf(5, firdes.low_pass(
        	lo_gain, samp_rate, 210000, 10000, firdes.WIN_RECTANGULAR, 6.76))
        self.blocks_wavfile_source_0 = blocks.wavfile_source('/home/gnuradio/Desktop/Records/SDRSharp_20190522_083545Z_737686889Hz_IQ-1-1s.wav', False)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, samp_rate,True)
        self.blocks_multiply_xx_0_0 = blocks.multiply_vcc(1)
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_float_to_complex_0 = blocks.float_to_complex(1)
        self.blocks_complex_to_mag_0_0 = blocks.complex_to_mag(1)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.band_pass_filter_0 = filter.fir_filter_ccf(1, firdes.band_pass(
        	1, samp_rate, subarrier_index*15000+1, subarrier_index*15000+(nb_subcarrier*15000), 10000, firdes.WIN_HAMMING, 6.76))
        self.analog_sig_source_x_0_0 = analog.sig_source_c(samp_rate, analog.GR_COS_WAVE, lo_freq, 1, 0)
        self.analog_noise_source_x_0 = analog.noise_source_c(analog.GR_GAUSSIAN, noise_gain, 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.nbiot_npss_sync_0, 'npss_sync'), (self.blocks_message_debug_0, 'print'))
        self.connect((self.analog_noise_source_x_0, 0), (self.band_pass_filter_0, 0))
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_multiply_xx_0_0, 1))
        self.connect((self.band_pass_filter_0, 0), (self.blocks_add_xx_0, 1))
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_throttle_0_0, 0))
        self.connect((self.blocks_complex_to_mag_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.blocks_complex_to_mag_0_0, 0), (self.qtgui_time_sink_x_0_0, 0))
        self.connect((self.blocks_float_to_complex_0, 0), (self.blocks_multiply_xx_0_0, 0))
        self.connect((self.blocks_multiply_xx_0_0, 0), (self.blocks_add_xx_0, 0))
        self.connect((self.blocks_throttle_0_0, 0), (self.low_pass_filter_0, 0))
        self.connect((self.blocks_wavfile_source_0, 0), (self.blocks_float_to_complex_0, 0))
        self.connect((self.blocks_wavfile_source_0, 1), (self.blocks_float_to_complex_0, 1))
        self.connect((self.low_pass_filter_0, 0), (self.rational_resampler_xxx_0, 0))
        self.connect((self.nbiot_custom_fft_0, 0), (self.blocks_complex_to_mag_0, 0))
        self.connect((self.nbiot_custom_fft_0, 0), (self.nbiot_nsss_decode_0, 0))
        self.connect((self.nbiot_npss_sync_0, 1), (self.nbiot_remove_cp_0, 0))
        self.connect((self.nbiot_npss_sync_0, 0), (self.qtgui_time_sink_x_1, 1))
        self.connect((self.nbiot_npss_sync_0, 1), (self.qtgui_time_sink_x_1, 0))
        self.connect((self.nbiot_nsss_decode_0, 0), (self.blocks_complex_to_mag_0_0, 0))
        self.connect((self.nbiot_remove_cp_0, 0), (self.nbiot_custom_fft_0, 0))
        self.connect((self.rational_resampler_xxx_0, 0), (self.nbiot_npss_sync_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_subarrier_index_0(self):
        return self.subarrier_index_0

    def set_subarrier_index_0(self, subarrier_index_0):
        self.subarrier_index_0 = subarrier_index_0

    def get_subarrier_index(self):
        return self.subarrier_index

    def set_subarrier_index(self, subarrier_index):
        self.subarrier_index = subarrier_index
        self.band_pass_filter_0.set_taps(firdes.band_pass(1, self.samp_rate, self.subarrier_index*15000+1, self.subarrier_index*15000+(self.nb_subcarrier*15000), 10000, firdes.WIN_HAMMING, 6.76))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.low_pass_filter_0.set_taps(firdes.low_pass(self.lo_gain, self.samp_rate, 210000, 10000, firdes.WIN_RECTANGULAR, 6.76))
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)
        self.band_pass_filter_0.set_taps(firdes.band_pass(1, self.samp_rate, self.subarrier_index*15000+1, self.subarrier_index*15000+(self.nb_subcarrier*15000), 10000, firdes.WIN_HAMMING, 6.76))
        self.analog_sig_source_x_0_0.set_sampling_freq(self.samp_rate)

    def get_noise_gain(self):
        return self.noise_gain

    def set_noise_gain(self, noise_gain):
        self.noise_gain = noise_gain
        self.analog_noise_source_x_0.set_amplitude(self.noise_gain)

    def get_nb_subcarrier_0(self):
        return self.nb_subcarrier_0

    def set_nb_subcarrier_0(self, nb_subcarrier_0):
        self.nb_subcarrier_0 = nb_subcarrier_0

    def get_nb_subcarrier(self):
        return self.nb_subcarrier

    def set_nb_subcarrier(self, nb_subcarrier):
        self.nb_subcarrier = nb_subcarrier
        self.band_pass_filter_0.set_taps(firdes.band_pass(1, self.samp_rate, self.subarrier_index*15000+1, self.subarrier_index*15000+(self.nb_subcarrier*15000), 10000, firdes.WIN_HAMMING, 6.76))

    def get_nb_frame(self):
        return self.nb_frame

    def set_nb_frame(self, nb_frame):
        self.nb_frame = nb_frame

    def get_lo_gain(self):
        return self.lo_gain

    def set_lo_gain(self, lo_gain):
        self.lo_gain = lo_gain
        self.low_pass_filter_0.set_taps(firdes.low_pass(self.lo_gain, self.samp_rate, 210000, 10000, firdes.WIN_RECTANGULAR, 6.76))

    def get_lo_freq(self):
        return self.lo_freq

    def set_lo_freq(self, lo_freq):
        self.lo_freq = lo_freq
        self.analog_sig_source_x_0_0.set_frequency(self.lo_freq)

    def get_iot_samp_rate(self):
        return self.iot_samp_rate

    def set_iot_samp_rate(self, iot_samp_rate):
        self.iot_samp_rate = iot_samp_rate
        self.qtgui_time_sink_x_1.set_samp_rate(self.iot_samp_rate)
        self.qtgui_time_sink_x_0_0.set_samp_rate(self.iot_samp_rate*896/960*12/128)
        self.qtgui_time_sink_x_0.set_samp_rate(self.iot_samp_rate*896/960*12/128)


def main(top_block_cls=top_block, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    print(str(tb.get_subarrier_index()))
    print(str(tb.get_nb_subcarrier()))

    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
