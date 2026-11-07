import sys
import subprocess
import re
import os
import shutil

TOP_MODULE_NAME = "dft"

def extract_metrics(design_name, text):
    lat = re.search(r"Latency.*?\|\s*(\d+)\s*\|\s*(\d+)\s*\|[^|]*\|[^|]*\|\s*(\d+)\s*\|\s*(\d+)\s*\|", text, re.DOTALL)
    tot = re.search(r"\|\s*Total\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|\s*(\d+)\s*\|", text)
    clk = re.search(r"\|\s*ap_clk\s*\|\s*[\d.]+\s*ns\|\s*([\d.]+)\s*ns", text)
    
    latency = lat.group(2) if lat else "NA"
    s_ii = lat.group(4) if lat else "NA"
    bram, dsp, ff, lut = (tot.group(1), tot.group(2), tot.group(3), tot.group(4)) if tot else ("NA", "NA", "NA", "NA")
    
    tp = "NA"
    if clk:
        try:
            c_val = float(s_ii) if s_ii != "NA" else (float(latency) if latency != "NA" else 0)
            if float(clk.group(1)) > 0 and c_val > 0:
                tp = f"{1000.0 / (float(clk.group(1)) * c_val):.3f}"
        except:
            pass

    return [design_name, latency, s_ii, bram, dsp, lut, ff, tp]

def extract_error(text):
    match = re.search(r"RMSE\(R\)\s+RMSE\(I\)\s*\n\s*([\d.eE+-]+)\s+([\d.eE+-]+)", text)
    if match:
        return match.group(1), match.group(2)
    return "NA", "NA"

def clean_question(q_dir):
    subprocess.run(["make", "clean"], cwd=q_dir, capture_output=True)
    reports_dir = os.path.join(q_dir, "Reports")
    if os.path.exists(reports_dir):
        shutil.rmtree(reports_dir)

def run_question(q_dir):
    reports_dir = os.path.join(q_dir, "Reports")
    os.makedirs(reports_dir, exist_ok=True)

    subprocess.run(["make", "clean"], cwd=q_dir, capture_output=True)
    test_out = subprocess.run(["make", "test"], cwd=q_dir, capture_output=True, text=True)
    hls_out = subprocess.run(["make", "hls"], cwd=q_dir, capture_output=True, text=True)
    rpt_out = subprocess.run(["make", "report"], cwd=q_dir, capture_output=True, text=True)

    hls_log = (hls_out.stdout or "") + (hls_out.stderr or "") + (rpt_out.stdout or "") + (rpt_out.stderr or "")
    test_log = (test_out.stdout or "") + (test_out.stderr or "")

    with open(os.path.join(reports_dir, f"{TOP_MODULE_NAME}_hls.log"), "w") as f:
        f.write(hls_log)

    with open(os.path.join(reports_dir, f"{TOP_MODULE_NAME}_test.log"), "w") as f:
        f.write(test_log)

    rpt_txt = ""
    rpt_src = os.path.join(q_dir, f"{TOP_MODULE_NAME}_csynth.rpt")
    rpt_dest = os.path.join(reports_dir, f"{TOP_MODULE_NAME}_csynth.rpt")
    
    if os.path.exists(rpt_src):
        shutil.move(rpt_src, rpt_dest)
        with open(rpt_dest, "r") as f: 
            rpt_txt = f.read()

    metrics = extract_metrics(q_dir, hls_log + "\n" + rpt_txt)
    rmse_r, rmse_i = extract_error(test_log)
    metrics.extend([rmse_r, rmse_i])
    
    headers = ["Design", "Latency", "Sample II", "BRAM", "DSP", "LUT", "FF", "Throughput (MHz)", "RMSE (R)", "RMSE (I)"]
    
    with open(os.path.join(reports_dir, "_table.md"), "w") as f:
        f.write("| " + " | ".join(headers) + " |\n")
        f.write("|" + "|".join(["---"] * len(headers)) + "|\n")
        f.write("| " + " | ".join(str(x) for x in metrics) + " |\n")

def main():
    q_arg = sys.argv[1] if len(sys.argv) > 1 else "all"
    action = sys.argv[2] if len(sys.argv) > 2 else "run"
    
    if q_arg == "all":
        targets = [d for d in os.listdir(".") if os.path.isdir(d) and d.startswith("Q")]
    else:
        targets = [q_arg] if os.path.isdir(q_arg) else []
    
    for t in sorted(targets):
        if action == "clean":
            clean_question(t)
        else:
            run_question(t)

if __name__ == "__main__":
    main()