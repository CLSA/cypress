from devices.spirometer.emr.output.base import SpirometerXML
from devices.spirometer.emr.output.result_parameter import ResultParameter

class Trial(SpirometerXML):
    field_map = {
        "Date": {"key": "trial_date", "type": str},
        "Number": {"key": "trial_number", "type": int},
        "Rank": {"key": "trial_rank", "type": int},
        "RankOriginal": {"key": "trial_rank_original", "type": int},
        "Accepted": {"key": "trial_accepted", "type": bool},
        "AcceptedOriginal": {"key": "trial_accepted_original", "type": bool},
        "ManualAmbientOverride": {"key": "trial_manual_ambient_override", "type": bool},
        "SamplingInterval": {"key": "sampling_interval", "type": float },
        # "SamplingValues":"values",
        "TimeZeroOffset": {"key": "time_zero_offset", "type": float },
        "DefaultVTPlotDrawEndIdx": {"key": "default_vt_plot_draw_end_index", "type": float }
    }

    def _parse_xml(self, root):
        for child in root:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)

        result_parameter_els = root.findall(".//ResultParameter")
        self.result_parameters = []
        for result_parameter_el in result_parameter_els:
            self.result_parameters.append(ResultParameter(result_parameter_el))

        channel_flow_el = root.find(".//ChannelFlow")
        for child in channel_flow_el:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)

        channel_volume_el = root.find(".//ChannelVolume")
        for child in channel_volume_el:
            if child.tag in self.field_map:
                self.set_attribute(child.tag, child.text)

    def get_results(self):
        return self.result_parameters